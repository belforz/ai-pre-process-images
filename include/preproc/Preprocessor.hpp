#ifndef PREPROCESSOR_HPP
#define PREPROCESSOR_HPP

#include <opencv2/opencv.hpp>
#include <optional>
#include <preproc/Metadata.hpp>

//  -------------------------- comment before public function -------------------------
/**
 * @brief Resume something
 *
 * Comments should be clear and concise, explaining the purpose of the function.
 *
 * @param description of the param description
 * @return result of the function
 * @throws std::runtime_error if an error occurs
 * @note Additional notes about the function, if necessary.
 */

//  cv::something something(const something& something)

//  -------------------------- comment before public function -------------------------

/**
 * @brief Loads an image from disk with extension and integrity validation.
 *
 * Accepts files in the following formats: .jpg, .jpeg, .png, .tiff, .webp.
 * Automatically converts .webp files to .png.
 * Rejects RAW files (.cr2, .nef, .arw) with a warning to the user.
 *
 * @param imagePath Full path to the image.
 * @return Loaded image as cv::Mat.
 * @throws std::runtime_error if the extension is invalid or the file is corrupted.
 */
cv::Mat uploadImage(const std::string &imagePath);

/**
 * @brief Resize the image proportionally if necessary and validate resolution quality.
 *
 * Keeps aspect ratio. Does not upscale small images.
 * Flags critical resolution cases based on pre-defined thresholds.
 *
 * @param imagem Input image (cv::Mat).
 * @param resolutionCritic Flag set to true if the image is considered critically low resolution.
 * @return Resized image (or original if resizing not required).
 * @throws std::runtime_error if the image is empty or resizing fails.
 */

cv::Mat resizeImage(const cv::Mat &image, processorState &isResolutionCritic, std::optional<std::string> &outputPath);

/**
 * @brief Corrects the image orientation based on EXIF data, if absent, uses heuristics
 * @param image Input image (cv::Mat).
 * @param isOrientationCorrected Flag set to true if the orientation was corrected.
 * @return Image with corrected orientation.
 * @throws std::runtime_error if the image is empty or orientation correction fails.
 */

cv::Mat correctImageOrientation(const cv::Mat &image, processorState &isOrientationCorrected, std::optional<int> exifOrientation = std::nullopt);

/**
 * @brief Converts the image to grayscale/RGB/HSV and also its structures.
 *
 * This function converts the input image to grayscale format, correcting RGB or normalizing HSV.
 *
 * @param image Input image (cv::Mat).
 * @return struct containing grayscale, RGB, and HSV images.
 * @throws std::runtime_error if the image is empty or conversion fails.
 */
struct convertedImage
{
    cv::Mat grayImage;
    cv::Mat rgbImage;
    cv::Mat hsvImage;
    cv::Mat labImage;
    cv::Mat ycrcbImage;
    cv::Mat luvImage;
};

cv::Mat toGray(const cv::Mat &image, convertedImage &converted);
cv::Mat toRGB(const cv::Mat &image, convertedImage &converted);
cv::Mat toHSV(const cv::Mat &image, convertedImage &converted);
cv::Mat toLab(const cv::Mat &image, convertedImage &converted);
cv::Mat toYCrCb(const cv::Mat &image, convertedImage &converted);
cv::Mat toLuv(const cv::Mat &image, convertedImage &converted);
convertedImage generateColorSpaces(const cv::Mat &image, processorState &state);

/**
 * @brief Normalizes the image by diagnosting brightness and contrast.
 Image must be in 8-bit format(convert all others), if pixels are on range [<5] or [>250], warning is issued.
 * @param image Input image (cv::Mat).
    * @return JSON with diagnostics information.
    * @throws std::runtime_error if the image is empty or normalization fails.
 */

cv::Mat normalizeImage(const cv::Mat &image, processorState &state);

/**
 * @brief Checks if the image is compressed based on file extension and binary header.
 * This function checks the file extension and reads the binary header to determine if the image is compressed.
 * It supports JPEG, WEBP, and other common formats.
 * @param image Input image (cv::Mat).
 * @param state Processor state to update compression status.
 * @param imagePathOrExt Path or extension of the image file.
 * @return cv::Mat containing the original image if compression is detected, or an empty cv
 * ::Mat if the image is empty or an error occurs.
 */

void IsCompressed(const cv::Mat &image, processorState &state, const std::string &imagePathOrExt);

/**
 * @brief Generates a copy of the image with a thumbnail flag. // THIS FUNCTION IS OPTIONAL
 *
 * This function creates a copy of the input image and sets a flag in the processorState
 * structure to indicate that a thumbnail has been generated and also its own structure.
 *
 * @param image Input image (cv::Mat).
 * @param thumbnailGenerated Flag to indicate if a thumbnail was generated.
 * @return Copy of the input image (cv::Mat).
 * @throws std::runtime_error if the image is empty or copying fails.
 */
struct thumbnailGenerated
{
    cv::Mat thumbnailImage;
    cv::Mat blurImage; // this wont be used in V1, but it is here for future use
};

cv::Mat generateCopy(const cv::Mat &image, processorState &thumbnailFlag);

/**
 * @brief Generates a blur copy of the image // THIS FUNCTION IS DISHABILITED FOR APP PURPOSES in V1.
 * This function creates a copy of the input image and applies varieties of blur calculations to inspect various
 * dimensions of the image, it stars to use GRAYSCALE to be more precisve with its results.
 *
 * @param image Input imag (cv::Mat).
 * @return copy of the input image(cv::Mat).
 * @throws std::runtime_error if the image is empty or copying fails.
 */

struct blurVarieties
{
    cv::Mat gaussian;
    cv::Mat median;
    cv::Mat bilateral;
    cv::Mat laplacian;
};

/** @brief Applies blur depending on Gaussian, Median, Bilateral & Laplacian to the image.
 * This function applies Gaussian blur to the input image and stores the result in the blurVarieties
 * structure.
 * @param image Input image (cv::Mat).
 * @param blur Reference to the blurVarieties structure to store the blurred image.
 * @return Blurred image (cv::Mat).
 * @throws std::runtime_error if the image is empty or blurring fails.
 */

cv::Mat toGaussian(const cv::Mat &image, blurVarieties &blur);
cv::Mat toMedian(const cv::Mat &image, blurVarieties &blur);
cv::Mat toBilateral(const cv::Mat &image, blurVarieties &blur);
cv::Mat toLaplacian(const cv::Mat &image, blurVarieties &blur);
blurVarieties generateBlurCopy(const cv::Mat &image);

#endif // PREPROCESSOR_HPP
