#include <opencv2/opencv.hpp>
#include "preproc/Preprocessor.hpp"
#include "Logger.hpp"
#include <string>
#include <iostream>

void testThumbnailGeneration(const std::vector<std::string>& imagesPath) {
    logger.log("Starting thumbnail generation process", LogLevel::SECTION);

    for (size_t i = 0; i < imagesPath.size(); ++i) {
        const auto& path = imagesPath[i];
        logger.log("Processing: " + path, LogLevel::INFO);

        cv::Mat image = cv::imread(path);
        if (image.empty()) {
            logger.log("Failed to load image: " + path, LogLevel::ERROR);
            continue;
        }

        PreprocessorState state;
        cv::Mat thumbnailCopy = generateCopy(image, state);

        if (!state.thumbnail_generated || thumbnailCopy.empty()) {
            logger.log("Thumbnail not generated for: " + path, LogLevel::WARNING);
            continue;
        }

        std::string outName = "./images/thumbs/thumbnail_" + std::to_string(i+1) + ".png";
        if (cv::imwrite(outName, thumbnailCopy)) {
            logger.log("Image saved to: " + outName, LogLevel::INFO);
        } else {
            logger.log("Failed to save thumbnail: " + outName, LogLevel::ERROR);
        }
    }

    logger.log("Thumbnail generation process finished.", LogLevel::SECTION);
}

int main() {
    std::vector<std::string> imagesPath = {
        "./images/img_8bit.png",
        "./images/img_media.jpg",
        "./images/img_rgb_original.jpg",
        "./images/webp.png",
    };

    try {
        testThumbnailGeneration(imagesPath);
    }
    catch (const std::exception &e) {
        logger.log(std::string("Exception in test: ") + e.what(), LogLevel::ERROR);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}