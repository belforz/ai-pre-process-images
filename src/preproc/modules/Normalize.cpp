#include <opencv2/opencv.hpp>
#include "preproc/Preprocessor.hpp"
#include "preproc/Metadata.hpp"
#include "Logger.hpp"
#include "pathsUtils.hpp"

cv::Mat normalizeImage(const cv::Mat &image, processorState &state)
{
    cv::Mat imgNormalized;

    try
    {
        if (image.empty())
        {
            logger.log("Input image is empty.", LogLevel::ERROR);
            throw std::runtime_error("Input image is empty.");
        }

        bool converted = false;

        if (image.type() == CV_8UC1 || image.type() == CV_8UC3)
        {
            imgNormalized = image.clone();
            logger.log("Image is already in 8-bit format.", LogLevel::INFO);
            state.is_normalized = false;
        }
        else if (image.depth() == CV_16U)
        {
            image.convertTo(imgNormalized, CV_8U, 255.0 / 65535.0);
            logger.log("Converted image from 16-bit to 8-bit.", LogLevel::INFO);
            converted = true;
            state.is_normalized = true;

        }
        else if (image.depth() == CV_32F)
        {
            cv::Mat temp;
            cv::normalize(image, temp, 0, 255, cv::NORM_MINMAX);
            temp.convertTo(imgNormalized, CV_8U);
            logger.log("Normalized float image to 8-bit using NORM_MINMAX.", LogLevel::INFO);
            converted = true;
            state.is_normalized = true;
        }
        else
        {
            logger.log("Unsupported image type for normalization.", LogLevel::ERROR);
            throw std::runtime_error("Unsupported image type for normalization.");
        }

    
        if (converted)
        {
            std::string baseDir = "./images/temp/normalized";
            pathutils::ensureDirectory(baseDir);
            std::string baseName = pathutils::getFilenameWithoutExtension(state.original_filename.empty() ? "image" : state.original_filename);
            std::string savePath = pathutils::join(baseDir, baseName + "_normalized_8bit.png");
            cv::imwrite(savePath, imgNormalized);
            logger.log("Normalized 8-bit image saved at: " + savePath, LogLevel::INFO);
        }

        cv::Mat gray;
        if (imgNormalized.channels() == 3)
        {
            cv::cvtColor(imgNormalized, gray, cv::COLOR_BGR2GRAY);
        }
        else
        {
            gray = imgNormalized;
        }

        // Explicit masks
        cv::Mat maskSaturated = (gray >= 250);
        cv::Mat maskUnderexposed = (gray <= 5);

        int saturated = cv::countNonZero(maskSaturated);
        int underexposed = cv::countNonZero(maskUnderexposed);

        // Update its state to explicit values
        state.saturated_pixel_count = saturated;
        state.underexposed_pixel_count = underexposed;
        state.has_satured_pixels = saturated > 0;
        state.has_underexposed_pixels = underexposed > 0;

        if (state.has_satured_pixels)
            logger.log("Image has saturated pixels. Count: " + std::to_string(saturated), LogLevel::WARNING);
        if (state.has_underexposed_pixels)
            logger.log("Image has underexposed pixels. Count: " + std::to_string(underexposed), LogLevel::WARNING);

        return imgNormalized;
    }
    catch (const std::exception &e)
    {
        state.error_msg = "Error during intensity normalization: " + std::string(e.what());
        logger.log(state.error_msg, LogLevel::ERROR);
        throw std::runtime_error(state.error_msg);
    }
}
