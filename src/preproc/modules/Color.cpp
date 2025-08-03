#include <opencv2/opencv.hpp>
#include <optional>
#include <string>
#include <iostream>
#include "preproc/Preprocessor.hpp"
#include "preproc/Metadata.hpp"
#include "pathsUtils.hpp"
#include "Logger.hpp"

cv::Mat toGray(const cv::Mat &image, convertedImage &converted) {
    cv::Mat grayImage;
    if (image.channels() == 3)
        cv::cvtColor(image, grayImage, cv::COLOR_BGR2GRAY);
    else
        grayImage = image.clone();
    logger.log("Converted image to Grayscale.", LogLevel::INFO);
    converted.grayImage = grayImage;
    return grayImage;
}

cv::Mat toRGB(const cv::Mat &image, convertedImage &converted) {
    cv::Mat rgbImage;
    if (image.channels() == 3)
        cv::cvtColor(image, rgbImage, cv::COLOR_BGR2RGB);
    else
        rgbImage = image.clone();
    logger.log("Converted image to RGB.", LogLevel::INFO);
    converted.rgbImage = rgbImage;
    return rgbImage;
}

cv::Mat toHSV(const cv::Mat &image, convertedImage &converted) {
    cv::Mat hsvImage;
    if (image.channels() == 3)
        cv::cvtColor(image, hsvImage, cv::COLOR_BGR2HSV);
    else
        hsvImage = image.clone();
    logger.log("Converted image to HSV.", LogLevel::INFO);
    converted.hsvImage = hsvImage;
    return hsvImage;
}

cv::Mat toLab(const cv::Mat &image, convertedImage &converted) {
    cv::Mat labImage;
    if (image.channels() == 3)
        cv::cvtColor(image, labImage, cv::COLOR_BGR2Lab);
    else
        labImage = image.clone();
    logger.log("Converted image to Lab.", LogLevel::INFO);
    converted.labImage = labImage;
    return labImage;
}

cv::Mat toYCrCb(const cv::Mat &image, convertedImage &converted) {
    cv::Mat ycrcbImage;
    if (image.channels() == 3)
        cv::cvtColor(image, ycrcbImage, cv::COLOR_BGR2YCrCb);
    else
        ycrcbImage = image.clone();
    logger.log("Converted image to YCrCb.", LogLevel::INFO);
    converted.ycrcbImage = ycrcbImage;
    return ycrcbImage;
}

cv::Mat toLuv(const cv::Mat &image, convertedImage &converted) {
    cv::Mat luvImage;
    if (image.channels() == 3)
        cv::cvtColor(image, luvImage, cv::COLOR_BGR2Luv);
    else
        luvImage = image.clone();
    logger.log("Converted image to Luv.", LogLevel::INFO);
    converted.luvImage = luvImage;
    return luvImage;
}



convertedImage generateColorSpaces(const cv::Mat &image, processorState &state) {
    convertedImage converted;
    try {
        if (image.empty()) {
            logger.log("Input image is empty.", LogLevel::ERROR);
            throw std::runtime_error("Input image is empty.");
        }

        logger.log("Generating color spaces for the image...", LogLevel::INFO);
        toGray(image, converted);
        toRGB(image, converted);
        toHSV(image, converted);
        toLab(image, converted);
        toYCrCb(image, converted);
        toLuv(image, converted);
        state.color_spaces_generated = true;

        std::string baseDir = "./images/temp/color_spaces";
        pathutils::ensureDirectory(baseDir);

        // 
        std::string baseName = pathutils::getFilenameWithoutExtension(state.original_filename.empty() ? "image" : state.original_filename);

        cv::imwrite(pathutils::join(baseDir, baseName + "_gray.png"), converted.grayImage);
        cv::imwrite(pathutils::join(baseDir, baseName + "_rgb.png"), converted.rgbImage);
        cv::imwrite(pathutils::join(baseDir, baseName + "_hsv.png"), converted.hsvImage);
        cv::imwrite(pathutils::join(baseDir, baseName + "_lab.png"), converted.labImage);
        cv::imwrite(pathutils::join(baseDir, baseName + "_ycrcb.png"), converted.ycrcbImage);
        cv::imwrite(pathutils::join(baseDir, baseName + "_luv.png"), converted.luvImage);

        logger.log("Color space images saved in " + baseDir, LogLevel::INFO);

        return converted;
    } catch (const std::exception &e) {
        logger.log(std::string("Error generating color spaces: ") + e.what(), LogLevel::ERROR);
        throw;
    }
}
