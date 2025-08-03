#include <opencv2/opencv.hpp>
#include "preproc/Preprocessor.hpp"
#include "Logger.hpp"
#include <string>
#include <iostream>

void testColorSpaces(const std::string& imagepath) {
    try {
        cv::Mat image = cv::imread(imagepath);
        if (image.empty()) {
            std::cerr << "Failed to load image: " << imagepath << std::endl;
            return;
        }

        logger.log("Color space conversion test started.", LogLevel::SECTION);

        auto state = processorState();

        convertedImage colorSpaces = generateColorSpaces(image, state);

        // Salva cada espaço de cor em um arquivo para inspeção
        cv::imwrite("./images/temp/img_gray.jpg", colorSpaces.grayImage);
        cv::imwrite("./images/temp/img_rgb.jpg", colorSpaces.rgbImage);
        cv::imwrite("./images/temp/img_hsv.jpg", colorSpaces.hsvImage);
        cv::imwrite("./images/temp/img_lab.jpg", colorSpaces.labImage);
        cv::imwrite("./images/temp/img_ycrcb.jpg", colorSpaces.ycrcbImage);
        cv::imwrite("./images/temp/img_luv.jpg", colorSpaces.luvImage);

        logger.log("All conversions saved in ./images/temp/", LogLevel::INFO);

        std::cout << "Test finished. Check ./images/temp/ for output images." << std::endl;
    } catch (const std::exception &e) {
        logger.log(std::string("Error during color space conversion: ") + e.what(), LogLevel::ERROR);
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}

int main() {
    testColorSpaces("./images/img_rgb_original.jpg");
    return 0;
}