#include <opencv2/opencv.hpp>
#include <optional>
#include <string>
#include <iostream>
#include "preproc/Preprocessor.hpp"
#include "preproc/Metadata.hpp"
#include "Logger.hpp"
#include "pathsUtils.hpp"

cv::Mat resizeImage(const cv::Mat &image, processorState &state, std::optional<std::string> &outputPath) {
    try {
        if (image.empty()) {
            logger.log("Input image is empty.", LogLevel::ERROR);
            state.is_image_valid = false;
            outputPath.reset();
            return cv::Mat();
        }

        int width = image.cols;
        int height = image.rows;
        int maxSide = std::max(width, height);

        state.is_image_valid = true;
        state.is_image_resized = false;
        state.is_resolution_critic = false;

        if (width < 640 || height < 480) {
            logger.log("Image resolution is critically low.", LogLevel::WARNING);
            state.is_resolution_critic = true;
            outputPath.reset();
            return image;
        } 

        if (maxSide != 640) {
            float scale = 640.0f / static_cast<float>(maxSide);
            int newWidth = static_cast<int>(width * scale);
            int newHeight = static_cast<int>(height * scale);
            cv::Mat resized;
            cv::resize(image, resized, cv::Size(newWidth, newHeight), 0, 0, cv::INTER_LANCZOS4);
            logger.log("Image resized to " + std::to_string(newWidth) + "x" + std::to_string(newHeight), LogLevel::INFO);
            state.is_image_resized = true;

            // Use pathutils 
            std::string resizedDir = "./images/temp/resized";
            pathutils::ensureDirectory(resizedDir);
            std::string baseName = pathutils::getFilenameWithoutExtension(state.original_filename.empty() ? "image" : state.original_filename);
            std::string filename = baseName + "_resized" + ".png";
            outputPath = pathutils::join(resizedDir, filename);
            cv::imwrite(*outputPath, resized);

            return resized;
        }

        outputPath.reset();
        return image;
    } catch (const std::exception &e) {
        logger.log(e.what(), LogLevel::ERROR);
        std::cerr << e.what() << '\n';
        outputPath.reset();
        return cv::Mat();
    }
}