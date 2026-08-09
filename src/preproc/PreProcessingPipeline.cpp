#include "preproc/PreProcessingPipeline.hpp"
#include "preproc/Preprocessor.hpp"
#include "Logger.hpp"
#include "preproc/MetadataSerializer.hpp"
#include "pathsUtils.hpp"
#include <filesystem>
#include <nlohmann/json.hpp>
#include <fstream>
#include <optional>
#include <regex>

namespace fs = std::filesystem;

void updateImagesIndex(const std::vector<std::tuple<std::string, std::string, int, std::string>> &imageEntries, const std::string &category = "")
{
    std::string indexPath = "local/images_index.json";
    nlohmann::json index;

    // Read existing index if it exists
    std::ifstream inFile(indexPath);
    if (inFile.is_open())
    {
        try
        {
            inFile >> index;
        }
        catch (const std::exception &)
        {
            index = nlohmann::json::object();
        }
        inFile.close();
    }
    else
    {
        index = nlohmann::json::object();
    }

    // Ensure "images" array exists
    if (!index.contains("images"))
    {
        index["images"] = nlohmann::json::array();
    }

    for (const auto &[path, type, exif, filename] : imageEntries)
    {
        // Check if already exists
        bool exists = false;
        for (const auto &img : index["images"])
        {
            if (img.contains("path") && img["path"] == path)
            {
                exists = true;
                break;
            }
        }
        if (!exists)
        {
            if (!category.empty())
            {
                // Categoria informada explicitamente (ex: category_code do payload,
                // ou --category do modo manual) tem prioridade sobre a inferencia por path.
                index["category"] = category;
                logger.log("Setting category to: " + category, LogLevel::INFO);
            }
            else if (!index.contains("category"))
            {
                std::string folderIndexPath = "home/belforz/dataset/";
                std::string currentFolderPath = folderIndexPath + path;

                std::regex rgx("dataset/([^/]+)");
                std::smatch matches;
                std::string categoryName = "unknown";

                if (std::regex_search(currentFolderPath, matches, rgx))
                {
                    categoryName = matches[1].str();
                }

                index["category"] = categoryName;
                logger.log("Setting dataset category to: " + categoryName, LogLevel::INFO);
            }
            logger.log("Adding to images index: " + path + " type: " + type, LogLevel::INFO);
            nlohmann::json newEntry = {
                {"path", path},
                {"type", type},
                {"exif_orientation", exif},
                {"filename", filename}};
            index["images"].push_back(newEntry);
        }
    }

    // Write back
    std::ofstream outFile(indexPath);
    if (outFile.is_open())
    {
        outFile << index.dump(4);
        outFile.close();
        logger.log("Updated images index: " + indexPath, LogLevel::INFO);
    }
    else
    {
        logger.log("Failed to write images index: " + indexPath, LogLevel::ERROR);
    }
}

int exifOrientationFromJson(const std::string &jsonPath)
{
    std::ifstream f(jsonPath);
    if (!f)
        return 1;
    nlohmann::json j;
    f >> j;
    if (j.contains("orientation"))
        return j["orientation"];
    return 1;
}

ImageMetadata runPreprocessingPipeline(const std::string &imagePath, std::optional<int> exifOrientation, const std::string &category)
{
    ImageMetadata metadata;
    PreprocessorState state;
    std::vector<std::tuple<std::string, std::string, int, std::string>> imageEntries;
    try
    {
        logger.log("Starting preprocessing pipeline for: " + imagePath, LogLevel::SECTION);
        cv::Mat image = uploadImage(imagePath);
        if (image.empty())
        {
            state.is_image_valid = false;
            logger.log("Failed to upload image: " + imagePath, LogLevel::ERROR);
            throw std::runtime_error("Failed to upload image: " + imagePath);
        }

        logger.log("Image uploaded successfully: " + imagePath, LogLevel::INFO);
        metadata.filename = fs::path(imagePath).filename().string();
        state.original_filename = metadata.filename;
        metadata.hash = computeSHA256(imagePath);
        metadata.width = image.cols;
        metadata.height = image.rows;
        metadata.aspect_ratio = static_cast<double>(image.cols) / image.rows;
        metadata.original_format = fs::path(imagePath).extension().string();
        metadata.image_type = "unknown";

        // Add original image
        imageEntries.emplace_back(pathutils::getAbsolutePath(imagePath), "original", exifOrientation.value_or(1), metadata.filename);

        // 2. RESIZE
        logger.log("Resizing image if necessary", LogLevel::INFO);
        std::optional<std::string> outputPath = std::nullopt;
        cv::Mat resize = resizeImage(image, state, outputPath);
        if (resize.empty())
        {
            state.is_image_valid = false;
            logger.log("Image resize failed, using original image.", LogLevel::WARNING);
            resize = image;
        }
        else
        {
            state.is_image_resized = true;
            logger.log("Image resized successfully.", LogLevel::INFO);
            if (outputPath)
            {
                imageEntries.emplace_back(pathutils::getAbsolutePath(*outputPath), "resized", exifOrientation.value_or(1), metadata.filename);
            }
        }

        if (state.is_image_resized)
        {
            metadata.width = resize.cols;
            metadata.height = resize.rows;
            metadata.aspect_ratio = static_cast<double>(resize.cols) / resize.rows;
        }

        // 3. NORMALIZE
        logger.log("Normalizing image", LogLevel::INFO);
        cv::Mat normalize = normalizeImage(resize, state);
        if (normalize.empty())
        {
            state.is_image_valid = false;
            logger.log("Image normalization failed, using original image.", LogLevel::WARNING);
            normalize = image;
        }
        else if (state.is_normalized)
        {
            logger.log("Image normalized successfully.", LogLevel::INFO);
        }
        else
        {
            logger.log("Image already in 8-bit format, no normalization needed.", LogLevel::INFO);
        }

        // 4. COLOR SPACES
        logger.log("Generating color spaces", LogLevel::INFO);
        convertedImage colorSpaces = generateColorSpaces(resize, state);
        colorSpaces.grayImage = colorSpaces.grayImage.empty() ? image : colorSpaces.grayImage;
        colorSpaces.rgbImage = colorSpaces.rgbImage.empty() ? image : colorSpaces.rgbImage;
        colorSpaces.hsvImage = colorSpaces.hsvImage.empty() ? image : colorSpaces.hsvImage;
        colorSpaces.labImage = colorSpaces.labImage.empty() ? image : colorSpaces.labImage;
        colorSpaces.ycrcbImage = colorSpaces.ycrcbImage.empty() ? image : colorSpaces.ycrcbImage;
        colorSpaces.luvImage = colorSpaces.luvImage.empty() ? image : colorSpaces.luvImage;

        if (colorSpaces.grayImage.empty() ||
            colorSpaces.rgbImage.empty() ||
            colorSpaces.hsvImage.empty() ||
            colorSpaces.labImage.empty() ||
            colorSpaces.ycrcbImage.empty() ||
            colorSpaces.luvImage.empty())
        {
            state.color_spaces_generated = false;
            logger.log("Failed to generate one or more color spaces, using original image.", LogLevel::WARNING);
        }
        else
        {
            state.color_spaces_generated = true;
            logger.log("Color spaces generated successfully.", LogLevel::INFO);
            // Add color space images
            std::string baseDir = "./images/temp/color_spaces";
            std::string baseName = pathutils::getFilenameWithoutExtension(metadata.filename);
            imageEntries.emplace_back(pathutils::getAbsolutePath(pathutils::join(baseDir, baseName + "_gray.png")), "color_gray", exifOrientation.value_or(1), metadata.filename);
            imageEntries.emplace_back(pathutils::getAbsolutePath(pathutils::join(baseDir, baseName + "_rgb.png")), "color_rgb", exifOrientation.value_or(1), metadata.filename);
            imageEntries.emplace_back(pathutils::getAbsolutePath(pathutils::join(baseDir, baseName + "_hsv.png")), "color_hsv", exifOrientation.value_or(1), metadata.filename);
            imageEntries.emplace_back(pathutils::getAbsolutePath(pathutils::join(baseDir, baseName + "_lab.png")), "color_lab", exifOrientation.value_or(1), metadata.filename);
            imageEntries.emplace_back(pathutils::getAbsolutePath(pathutils::join(baseDir, baseName + "_ycrcb.png")), "color_ycrcb", exifOrientation.value_or(1), metadata.filename);
            imageEntries.emplace_back(pathutils::getAbsolutePath(pathutils::join(baseDir, baseName + "_luv.png")), "color_luv", exifOrientation.value_or(1), metadata.filename);
        }

        // 5. ORIENTATION
        logger.log("Correcting image orientation if necessary", LogLevel::INFO);
        int convertedExifOrientation = 1;
        if (exifOrientation.has_value())
        {
            convertedExifOrientation = exifOrientation.value();
        }
        else
        {
            std::string exifJsonPath = imagePath + ".exif.json";
            convertedExifOrientation = exifOrientationFromJson(exifJsonPath);
        }
        cv::Mat orientedImage = correctImageOrientation(resize, state, convertedExifOrientation);
        if (orientedImage.empty())
        {
            state.is_orientation_corrected = false;
            logger.log("Image orientation correction failed, using original image.", LogLevel::WARNING);
            orientedImage = resize;
        }
        else if (convertedExifOrientation == 3 || convertedExifOrientation == 6 || convertedExifOrientation == 8 || state.is_orientation_corrected)
        {
            state.is_orientation_corrected = true;
            logger.log("Image orientation corrected successfully.", LogLevel::INFO);
            // Add oriented image
            std::string orientedDir = "images/temp/oriented";
            std::string baseName = pathutils::getFilenameWithoutExtension(metadata.filename);
            std::string filename = baseName + "_oriented_exif_" + std::to_string(convertedExifOrientation) + ".png";
            std::string orientedPath = pathutils::join(orientedDir, filename);
            imageEntries.emplace_back(pathutils::getAbsolutePath(orientedPath), "oriented", convertedExifOrientation, metadata.filename);
        }
        else
        {
            state.is_orientation_corrected = false;
            logger.log("No orientation correction applied (EXIF: " + std::to_string(convertedExifOrientation) + ")", LogLevel::INFO);
        }

        // 6. COMPRESSION
        logger.log("Checking image compression", LogLevel::INFO);
        IsCompressed(resize, state, metadata.original_format);
        if (state.has_compressed_image)
        {
            logger.log("Image is compressed: " + state.compression_type, LogLevel::INFO);
        }
        else
        {
            logger.log("Image is not compressed.", LogLevel::INFO);
        }

        // 7. BLUR (SKIPPED)

        // 8. THUMBNAIL
        logger.log("Generating thumbnail", LogLevel::INFO);
        cv::Mat thumbnail = generateCopy(resize, state);
        if (thumbnail.empty())
        {
            logger.log("Thumbnail generation failed, using original image.", LogLevel::WARNING);
            thumbnail = resize;
        }
        else
        {
            logger.log("Thumbnail generated successfully.", LogLevel::INFO);
            // Add thumbnail
            std::string thumbDir = "./images/temp/thumbs";
            std::string baseName = pathutils::getFilenameWithoutExtension(metadata.filename);
            int newWidth = 128;
            int newHeight = static_cast<int>(resize.rows * (static_cast<float>(newWidth) / resize.cols));
            std::string thumbPath = pathutils::join(thumbDir, baseName + "_thumbnail_" + std::to_string(newWidth) + "x" + std::to_string(newHeight) + ".png");
            imageEntries.emplace_back(pathutils::getAbsolutePath(thumbPath), "thumbnail", exifOrientation.value_or(1), metadata.filename);
        }
    }
    catch (const std::exception &e)
    {
        logger.log("Error in preprocessing pipeline: " + std::string(e.what()), LogLevel::ERROR);
        throw;
    }
    metadata.preproc_state = state;
    saveMetadataAsJson(metadata, "local/json/" + metadata.filename + ".metadata.json");
    updateImagesIndex(imageEntries, category);
    return metadata;
}
