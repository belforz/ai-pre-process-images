#include "preproc/PreProcessingPipeline.hpp"
#include "preproc/Preprocessor.hpp"
#include "Logger.hpp"
#include "preproc/MetadataSerializer.hpp"
#include "pathsUtils.hpp"
#include <filesystem>
#include <nlohmann/json.hpp>
#include <fstream>
#include <optional>

namespace fs = std::filesystem;

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

ImageMetadata runPreprocessingPipeline(const std::string &imagePath, std::optional<int> exifOrientation)
{
    ImageMetadata metadata;
    PreprocessorState state;
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
        }

        // 3. NORMALIZE
        logger.log("Normalizing image", LogLevel::INFO);
        cv::Mat normalize = normalizeImage(image, state);
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
        convertedImage colorSpaces = generateColorSpaces(image, state);
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
        }

        // 5. ORIENTATION
        logger.log("Correcting image orientation if necessary", LogLevel::INFO);
        int convertedExifOrientation = 1;
        if (exifOrientation.has_value()) {
            convertedExifOrientation = exifOrientation.value();
        } else {
            std::string exifJsonPath = imagePath + ".exif.json";
            convertedExifOrientation = exifOrientationFromJson(exifJsonPath);
        }
        cv::Mat orientedImage = correctImageOrientation(image, state, convertedExifOrientation);
        if (orientedImage.empty())
        {
            state.is_orientation_corrected = false;
            logger.log("Image orientation correction failed, using original image.", LogLevel::WARNING);
            orientedImage = image;
        }
        else if (convertedExifOrientation == 3 || convertedExifOrientation == 6 || convertedExifOrientation == 8 || state.is_orientation_corrected) {
            state.is_orientation_corrected = true;
            logger.log("Image orientation corrected successfully.", LogLevel::INFO);
        }
        else
        {
            state.is_orientation_corrected = false;
            logger.log("No orientation correction applied (EXIF: " + std::to_string(convertedExifOrientation) + ")", LogLevel::INFO);
        }

        // 6. COMPRESSION
        logger.log("Checking image compression", LogLevel::INFO);
        IsCompressed(image, state, metadata.original_format);
        if(state.has_compressed_image)
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
        cv::Mat thumbnail = generateCopy(image, state);
        if (thumbnail.empty())
        {
            logger.log("Thumbnail generation failed, using original image.", LogLevel::WARNING);
            thumbnail = image;
        }
        else
        {
            logger.log("Thumbnail generated successfully.", LogLevel::INFO);
        }

    }
    catch (const std::exception &e)
    {
        logger.log("Error in preprocessing pipeline: " + std::string(e.what()), LogLevel::ERROR);
        throw;
    }
    saveMetadataAsJson(metadata, "local/json/" + metadata.filename + ".metadata.json");
    return metadata;
}
