#include <opencv2/opencv.hpp>
#include "preproc/Preprocessor.hpp"
#include "preproc/Metadata.hpp"
#include "Logger.hpp"
#include "pathsUtils.hpp"



cv::Mat uploadImage(const std::string &imagePath)
{
    std::string path = pathutils::sanitize(imagePath);

    // for binary via py
    // std::vector<uchar> buffer;

    // cv::Mat image = cv::imdecode(buffer, cv::IMREAD_UNCHANGED);

    // if((image.empty()) || (image.data ==nullptr)) {
    //     throw std::runtime_error("Failed to upload image: " + path + ". The file may be corrupted or the format is not supported.");
    // }

    if (!pathutils::isValidPath(path)) {
        logger.log("Invalid or unsafe image path: " + imagePath, LogLevel::ERROR);
        throw std::runtime_error("Invalid or unsafe image path: " + imagePath);
    }

    if (!pathutils::fileExists(path)) {
        logger.log("File does not exist: " + path, LogLevel::ERROR);
        throw std::runtime_error("File does not exist: " + path);
    }

    try {
        bool isWebp = path.size() >= 5 && path.compare(path.size() - 5, 5, ".webp") == 0;
        if (isWebp) {
            cv::Mat webpImage = cv::imread(path, cv::IMREAD_UNCHANGED);
            if (webpImage.empty()) {
                logger.log("Failed to upload image: " + path, LogLevel::ERROR);
                throw std::runtime_error("Failed to upload image: " + path);
            }
            std::string pngPath = path.substr(0, path.size() - 5) + ".png";
            logger.log("Converting .webp to .png: " + pngPath, LogLevel::INFO);
            bool success = cv::imwrite(pngPath, webpImage);
            if (!success) {
                logger.log("Failed to convert .webp to .png: " + pngPath, LogLevel::ERROR);
                throw std::runtime_error("Failed to convert .webp to .png: " + pngPath);
            }
            path = pngPath;
        }

        cv::Mat image = cv::imread(path, cv::IMREAD_UNCHANGED);
        if (image.empty() || image.data == nullptr) {
            logger.log("Failed to upload image: " + path, LogLevel::ERROR);
            throw std::runtime_error("Failed to upload image: " + path);
        }

        logger.log("Image uploaded successfully: " + pathutils::getAbsolutePath(path), LogLevel::INFO);
        return image;
    }
    catch (const std::exception &e) {
        logger.log(e.what(), LogLevel::ERROR);
        std::cerr << e.what() << '\n';
        return cv::Mat();
    }
}