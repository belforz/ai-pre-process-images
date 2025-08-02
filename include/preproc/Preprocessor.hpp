
#ifndef PREPROCESSOR_HPP
#define PREPROCESSOR_HPP

#include <opencv2/opencv.hpp>
#include <optional>


/**
 * @brief Represents the state of the preprocessor during image processing.
 *
 * This structure holds flags that indicate the status of various preprocessing steps.
 * for the technician understands the current state of the image was processed or not.
 */
 struct PreprocessorState {
     bool isImageValid = false;
     bool isImageResized = false;
     bool isResolutionCritic = false;
     bool hasEXIFData = false;
     bool isOrientationCorrected = false;
     bool hasCompressedImage = false;
     bool thumbnailGenerated = false;

     std::string errorMsg = "";
 };
using processorState = PreprocessorState;


/**
 * @brief Represents metadata associated with an image.
 *
 * This structure holds information about the image such as filename, hash, dimensions,
 * aspect ratio, original format, and type. It also includes flags indicating the state
 * of preprocessing.
 */

struct ImageMetadata {
    std::string filename;
    std::string hash; // SHA-256
    int width;
    int height;
    double aspectRatio;
    std::string originalFormat;
    std::string imageType; 
    PreprocessorState flags;

};

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
cv::Mat uploadImage(const std::string& imagePath);

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

cv::Mat resizeImage(const cv::Mat& image, processorState& isResolutionCritic, std::optional<std::string> &outputPath );



/**
  * @brief Corrects the image orientation based on EXIF data, if absent, uses heuristics
  * @param image Input image (cv::Mat).
  * @param isOrientationCorrected Flag set to true if the orientation was corrected.
  * @return Image with corrected orientation.
  * @throws std::runtime_error if the image is empty or orientation correction fails.
  */

cv::Mat correctImageOrientation(const cv::Mat& image, processorState& isOrientationCorrected, std::optional<int> exifOrientation = std::nullopt);

/**
 * @brief Converts the image to grayscale/RGB/HSV and also its structures.
 *
 * This function converts the input image to grayscale format, correcting RGB or normalizing HSV.
 *
 * @param image Input image (cv::Mat).
 * @return struct containing grayscale, RGB, and HSV images.
 * @throws std::runtime_error if the image is empty or conversion fails.
 */
struct convertedImage {
    cv::Mat grayImage;
    cv::Mat rgbImage;
    cv::Mat hsvImage;
};

cv::Mat toGray(const cv::Mat& image);
cv::Mat toRGB(const cv::Mat& image);
cv::Mat toHSV(const cv::Mat& image);
convertedImage generateColorSpaces(const cv::Mat& image);


/**
 * @brief Normalizes the image by diagnosting brightness and contrast.
 Image must be in 8-bit format(convert all others), if pixels are on range [<5] or [>250], warning is issued.
 * @param image Input image (cv::Mat).
    * @return JSON with diagnostics information.
    * @throws std::runtime_error if the image is empty or normalization fails.
 */

cv::Mat normalizeImage(const cv::Mat& image);


/**
 * @brief Checks if the image is compressed based on its extension and MIME type.
 *
 * This function determines if the image is compressed by checking its file extension
 * and optionally its MIME type. It sets a flag in the processorState structure.
 *
 * @param image Input image (cv::Mat).
 * @param compressedFlag Flag to indicate if the image is compressed.
 * @param imageExtension File extension of the image.
 * @param MIME Optional MIME type of the image.
 * @return Compressed image (cv::Mat) or original if not compressed.
 * @throws std::runtime_error if the image is empty or compression check fails.
 */


cv::Mat IsCompressed(const cv::Mat& image, processorState& compressedFlag, const std::string& imageExtension, const std::optional<cv::Mat>& MIME );

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
struct thumbnailGenerated{
    cv::Mat thumbnailImage;
    cv::Mat blurImage; // this wont be used in V1, but it is here for future use
    
};

cv::Mat generateCopy(const cv::Mat& image, processorState& thumbnailFlag);

#endif // PREPROCESSOR_HPP




















   


