#include <opencv2/opencv.hpp>
#include "preproc/Preprocessor.hpp"
#include "preproc/Metadata.hpp"
#include "Logger.hpp"

cv::Mat normalizeImage(const cv::Mat &image, processorState &state)
{
    cv::Mat imgNormalized;

    try
    {
        if (image.empty())
        {
            logger.log("Input image is empty.", LogLevel::ERROR);
            throw std::runtime_error("Input image is empty.");
        }

        if (image.type() == CV_8UC1 || image.type() == CV_8UC3)
        {
            imgNormalized = image.clone();
            logger.log("Image is already in 8-bit format.", LogLevel::INFO);
        }
        else if (image.depth() == CV_16U)
        {
            image.convertTo(imgNormalized, CV_8U, 255.0 / 65535.0);
            logger.log("Converted image from 16-bit to 8-bit.", LogLevel::INFO);
        }
        else if (image.depth() == CV_32F)
        {
            cv::Mat temp;
            cv::normalize(image, temp, 0, 255, cv::NORM_MINMAX);
            temp.convertTo(imgNormalized, CV_8U);
            logger.log("Normalized float image to 8-bit using NORM_MINMAX.", LogLevel::INFO);
        }
        else
        {
            logger.log("Unsupported image type for normalization.", LogLevel::ERROR);
            throw std::runtime_error("Unsupported image type for normalization.");
        }

        cv::Mat gray;
        if (imgNormalized.channels() == 3)
        {
            cv::cvtColor(imgNormalized, gray, cv::COLOR_BGR2GRAY);
        }
        else
        {
            gray = imgNormalized;
        }

        int saturated = cv::countNonZero(gray > 250);
        int underexposed = cv::countNonZero(gray < 5);

        state.saturated = saturated > 0 ? "Image has saturated pixels." : "";
        state.underexposed = underexposed > 0 ? "Image has underexposed pixels." : "";

        if (!state.saturated.empty())
            logger.log(state.saturated, LogLevel::WARNING);
        if (!state.underexposed.empty())
            logger.log(state.underexposed, LogLevel::WARNING);

        return imgNormalized;
    }
    catch (const std::exception &e)
    {
        state.error_msg = "Error during intensity normalization: " + std::string(e.what());
        logger.log(state.error_msg, LogLevel::ERROR);
        throw std::runtime_error(state.error_msg);
    }
}
