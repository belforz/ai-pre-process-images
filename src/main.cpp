#include <opencv2/opencv.hpp>
#include <iostream>
#include "preproc/Preprocessor.hpp"

int main() {
    std::string path = "./images/webp.webp";
    cv::Mat img = uploadImage(path);

    if(!img.empty()){
        std::cout << "Image uploaded successfully!" << std::endl;
        std::cout << "Image size: " << img.size() << std::endl;
    } else {
        std::cerr << "Failed to upload image." << std::endl;
    }

    return 0;
}
