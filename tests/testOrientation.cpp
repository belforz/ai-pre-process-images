#include <opencv2/opencv.hpp>
#include "preproc/Preprocessor.hpp"
#include "Logger.hpp"
#include <optional>
#include <vector>
#include <string>
#include <iostream>

void testOrientation() {
    std::vector<std::pair<std::string, std::optional<int>>> images = {
        {"./images/exif_1.jpg", 1},
        {"./images/exif_6.jpg", 6},
        {"./images/exif_3.jpg", 3},
        {"./images/exif_8.jpg", 8},
        {"./images/heuristic_no_face.jpg", std::nullopt}
    };

    for (size_t i = 0; i < images.size(); ++i) {
        const auto& [path, exifValue] = images[i];
        cv::Mat image = cv::imread(path);

        std::cout << "Testing orientation for: " << path << std::endl;

        if (image.empty()) {
            std::cerr << "  Failed to load image." << std::endl;
            continue;
        }

        processorState state;
        cv::Mat corrected = correctImageOrientation(image, state, exifValue);

        std::string outName = "./images/temp/corrected_" + std::to_string(i+1) + ".jpg";
        cv::imwrite(outName, corrected);
        logger.log("Image saved to: " + outName, LogLevel::INFO);

        std::cout << "  Processed -> " << outName
                  << " | Orientation corrected? " << (state.isOrientationCorrected ? "Yes" : "No") << std::endl;
    }
}

int main() {
    testOrientation();
    return 0;
}
