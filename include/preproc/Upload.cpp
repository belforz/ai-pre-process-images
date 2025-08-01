#include <opencv2/opencv.hpp>
#include "Preprocessor.hpp"
#include "Logger.hpp"

Logger logger;


cv::Mat uploadImage(const std::string &imagePath)
{
    // for local purposes.
    std::string path = imagePath;
    // for binary via py
    // std::vector<uchar> buffer;

    // cv::Mat image = cv::imdecode(buffer, cv::IMREAD_UNCHANGED);

    // if((image.empty()) || (image.data ==nullptr)) {
    //     throw std::runtime_error("Failed to upload image: " + path + ". The file may be corrupted or the format is not supported.");
    // }

    try
    {
        bool isWebp = path.size() >= 5 && path.compare(path.size() - 5, 5, ".webp") == 0;
        if (isWebp)
        {
            cv::Mat webpImage = cv::imread(path, cv::IMREAD_UNCHANGED);
            if (webpImage.empty())
                throw std::runtime_error("Failed to upload image: " + path + ". The file may be corrupted or the format is not supported.");

            std::string pngPath = path.substr(0, path.size() - 5) + ".png";
            logger.log("Converting .webp to .png: " + pngPath, LogLevel::INFO);
            bool sucess = cv::imwrite(pngPath, webpImage);
            if (!sucess)
                throw std::runtime_error("Failed to convert .webp to .png: " + pngPath);

            path = pngPath;
        }

        cv::Mat image = cv::imread(path, cv::IMREAD_UNCHANGED);
        logger.log("Image uploaded successfully: " + path, LogLevel::INFO);
        if ((image.empty()) || (image.data == nullptr))
            throw std::runtime_error("Failed to upload image: " + path + ". The file may be corrupted or the format is not supported.");

        return image;
    }

    catch (const std::exception &e)
    {
        logger.log(e.what(), LogLevel::ERROR);
        std::cerr << e.what() << '\n';
        return cv::Mat();
    }
}