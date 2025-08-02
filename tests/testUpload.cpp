#include <opencv2/opencv.hpp>
#include <iostream>
#include "preproc/Preprocessor.hpp"

void testUpload() {
    std::vector<std::string> paths = {
        "./images/webp.webp",
        "./images/img_alta.jpg",
        "./images/img_baixa.jpg",
        "./images/img_media.jpg"
    };

    for (const auto& path : paths) {
        cv::Mat img = uploadImage(path);

        std::cout << "Testing upload for: " << path << std::endl;
        if (!img.empty()) {
            std::cout << "  Image uploaded successfully!" << std::endl;
            std::cout << "  Image size: " << img.size() << std::endl;
        } else {
            std::cerr << "  Failed to upload image." << std::endl;
        }
    }
}

int main() {
    testUpload();
    return 0;
}