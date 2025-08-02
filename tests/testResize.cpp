#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include "preproc/Preprocessor.hpp"

void testResize() {
    std::vector<std::string> paths = {
        "./images/img_alta.jpg",
        "./images/img_baixa.jpg",
        "./images/img_media.jpg",
        "./images/webp.png",
    };

    std::vector<cv::Mat> images;
    std::vector<cv::Mat> resizedImages;
    std::vector<processorState> states(paths.size());
    int resizedCount = 0;

    for (size_t i = 0; i < paths.size(); ++i) {
        cv::Mat img = uploadImage(paths[i]);
        images.push_back(img);

        if (!img.empty()) {
            std::cout << "Processing " << paths[i] << "..." << std::endl;
            std::cout << "  Image uploaded successfully!" << std::endl;

            std::optional<std::string> outputPath;
            cv::Mat resized = resizeImage(img, states[i], outputPath);
            resizedImages.push_back(resized);

            if (states[i].isImageResized) {
                std::cout << "  Image was resized." << std::endl;
                if (outputPath) {
                    cv::imwrite(*outputPath, resized);
                    std::cout << "  Saved as: " << *outputPath << std::endl;
                }
                resizedCount++;
            } else {
                std::cout << "  Image was not resized." << std::endl;
            }
        } else {
            std::cerr << "  Failed to upload image." << std::endl;
        }
    }

    std::cout << "\nSummary: " << resizedCount << " of " << paths.size() << " images were resized." << std::endl;
}

// Para rodar como executável de teste:
int main() {
    testResize();
    return 0;
}