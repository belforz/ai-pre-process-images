#include <opencv2/opencv.hpp>
#include <optional>
#include <string>
#include <iostream>
#include "preproc/Preprocessor.hpp"
#include "preproc/Metadata.hpp"
#include "Logger.hpp"
#include "pathsUtils.hpp"



// cv::Mat toGaussian(const cv::Mat &image, blurVarieties &blur)
// {
//     if (image.empty())
//     {
//         logger.log("Input image is empty.", LogLevel::ERROR);
//         return;
//     }
//     cv::GaussianBlur(image, blur.gaussian, cv::Size(5, 5), 0);
//     logger.log("Gaussian blur applied successfully.", LogLevel::INFO);
//     return blur.gaussian;
// }

// cv::Mat toMedian(const cv::Mat &image, blurVarieties &blur)
// {
//     if (image.empty())
//     {
//         logger.log("Input image is empty.", LogLevel::ERROR);
//         return;
//     }
//     cv::medianBlur(image, blur.median, 5);
//     logger.log("Median blur applied successfully.", LogLevel::INFO);
//     return blur.median;
// }

// cv::Mat toBilateral(const cv::Mat &image, blurVarieties &blur)
// {
//     if (image.empty())
//     {
//         logger.log("Input image is empty.", LogLevel::ERROR);
//         return;
//     }
//     cv::bilateralFilter(image, blur.bilateral, 9, 75, 75);
//     logger.log("Bilateral blur applied successfully.", LogLevel::INFO);
//     return blur.bilateral;
// }

// cv::Mat toLaplacian(const cv::Mat &image, blurVarieties &blur)
// {
//     if (image.empty())
//     {
//         logger.log("Input image is empty.", LogLevel::ERROR);
//         return;
//     }
//     cv::Laplacian(image, blur.laplacian, CV_16S, 3);
//     cv::convertScaleAbs(blur.laplacian, blur.laplacian);
//     logger.log("Laplacian blur applied successfully.", LogLevel::INFO);
//     return blur.laplacian;
// }

cv::Mat generateCopy(const cv::Mat &image, processorState &thumbnailFlag)
{
    try
    {
        if (image.empty())
        {
            logger.log("Input image is empty.", LogLevel::ERROR);
            return cv::Mat();
        }

        int newWidth = 128;
        int originalWidth = image.cols;
        int originalHeight = image.rows;

        int newHeight = static_cast<int>(originalHeight * (static_cast<float>(newWidth) / originalWidth));

        cv::Mat thumbnail;
        cv::resize(image, thumbnail, cv::Size(newWidth, newHeight), 0, 0, cv::INTER_AREA);

        
        std::string thumbDir = "./images/temp/thumbs";
        pathutils::ensureDirectory(thumbDir);
        std::string baseName = pathutils::getFilenameWithoutExtension(
            thumbnailFlag.original_filename.empty() ? "image" : thumbnailFlag.original_filename
        );
        std::string thumbPath = pathutils::join(
            thumbDir,
            baseName + "_thumbnail_" + std::to_string(newWidth) + "x" + std::to_string(newHeight) + ".png"
        );
        cv::imwrite(thumbPath, thumbnail);
    

        logger.log("Thumbnail generated successfully", LogLevel::INFO);
        thumbnailFlag.thumbnail_generated = true;
        return thumbnail;
    }
    catch (const std::exception &e)
    {
        thumbnailFlag.error_msg = "Error during thumbnail generation: " + std::string(e.what());
        logger.log(thumbnailFlag.error_msg, LogLevel::ERROR);
        throw std::runtime_error(thumbnailFlag.error_msg);
    }
}

// blurVarieties generateBlurCopy(const cv::Mat &image)
// {
//     blurVarieties blurCopy;

//     try
//     {
//         if (image.empty())
//         {
//             logger.log("Input image is empty.", LogLevel::ERROR);
//             return;
//         }
//         logger.log("Generating blur copy of the image...", LogLevel::INFO);
//         toGaussian(image, blurCopy);
//         toMedian(image, blurCopy);
//         toBilateral(image, blurCopy);
//         toLaplacian(image, blurCopy);
//     }
//     catch (const std::exception &e)
//     {
//         logger.log(std::string("Error generating blur copy: ") + e.what(), LogLevel::ERROR);
//         throw;
//     }
// }
