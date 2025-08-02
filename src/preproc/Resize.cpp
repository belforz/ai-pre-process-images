#include <opencv2/opencv.hpp>
#include <optional>
#include <string>
#include <iostream>
#include "preproc/Preprocessor.hpp"
#include "Logger.hpp"

cv::Mat resizeImage(const cv::Mat &image, processorState &state, std::optional<std::string> &outputPath) {
    try {
        if (image.empty()) {
            logger.log("Input image is empty.", LogLevel::ERROR);
            state.isImageValid = false;
            outputPath.reset();
            return cv::Mat();
        }

        int width = image.cols;
        int height = image.rows;
        int maxSide = std::max(width, height);

        state.isImageValid = true;
        state.isImageResized = false;
        state.isResolutionCritic = false;

        if (width < 640 || height < 480) {
            logger.log("Image resolution is critically low.", LogLevel::WARNING);
            state.isResolutionCritic = true;
            outputPath.reset();
            return image;
        }

        if (maxSide < 1024) {
            float scale = 1024.0f / static_cast<float>(maxSide);
            int newWidth = static_cast<int>(width * scale);
            int newHeight = static_cast<int>(height * scale);
            cv::Mat resized;
            cv::resize(image, resized, cv::Size(newWidth, newHeight), 0, 0, cv::INTER_LANCZOS4);
            logger.log("Image resized to " + std::to_string(newWidth) + "x" + std::to_string(newHeight), LogLevel::INFO);
            state.isImageResized = true;
            outputPath = "./images/resized_output_" + std::to_string(width) + "x" + std::to_string(height) + ".png";
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