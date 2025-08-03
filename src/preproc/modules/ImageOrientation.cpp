#include <opencv2/opencv.hpp>
#include <optional>
#include <string>
#include <iostream>
#include "preproc/Preprocessor.hpp"
#include "preproc/Metadata.hpp"
#include "Logger.hpp"
#include "pathsUtils.hpp"

const std::string FACE_CASCADE_PATH = "/usr/share/opencv4/haarcascades/haarcascade_frontalface_default.xml";

cv::Mat correctImageOrientation(const cv::Mat& image, processorState& state, std::optional<int> exifOrientation) {
    cv::Mat correctedImage = image.clone();
    try {

        if(image.empty()){
            state.error_msg = "Input image is empty.";
            logger.log(state.error_msg, LogLevel::ERROR);
            return cv::Mat();
        }

        bool saved = false;

        std::string baseName = pathutils::getFilenameWithoutExtension(state.original_filename.empty() ? "image" : state.original_filename);

        if (exifOrientation.has_value()) {
            std::string orientedDir = "images/temp/oriented";
            pathutils::ensureDirectory(orientedDir);
            std::string filename = baseName + "_oriented_exif_" + std::to_string(exifOrientation.value()) + ".png";
            std::string savePath = pathutils::join(orientedDir, filename);

            switch (exifOrientation.value()) {
                case 1:
                    // Normal
                    logger.log("EXIF 1: Normal orientation, no correction applied.", LogLevel::INFO);
                    break;
                case 2:
                    // Horizontal flip
                    cv::flip(correctedImage, correctedImage, 1);
                    state.is_orientation_corrected = true;
                    logger.log("Image orientation corrected: horizontal flip (EXIF 2)", LogLevel::INFO);
                    cv::imwrite(savePath, correctedImage);
                    break;
                case 3:
                    // Rotate 180 degrees
                    cv::rotate(correctedImage, correctedImage, cv::ROTATE_180);
                    state.is_orientation_corrected = true;
                    logger.log("Image orientation corrected: 180 degrees rotation (EXIF 3)", LogLevel::INFO);
                    cv::imwrite(savePath, correctedImage);
                    break;
                case 4:
                    // Vertical flip
                    cv::flip(correctedImage, correctedImage, 0);
                    state.is_orientation_corrected = true;
                    logger.log("Image orientation corrected: vertical flip (EXIF 4)", LogLevel::INFO);
                    cv::imwrite(savePath, correctedImage);
                    break;
                case 5:
                    // Horizontal flip + 90° CCW
                    cv::flip(correctedImage, correctedImage, 1);
                    cv::rotate(correctedImage, correctedImage, cv::ROTATE_90_COUNTERCLOCKWISE);
                    state.is_orientation_corrected = true;
                    logger.log("Image orientation corrected: horizontal flip + 90° CCW (EXIF 5)", LogLevel::INFO);
                    cv::imwrite(savePath, correctedImage);
                    break;
                case 6:
                    // 90° CCW
                    cv::rotate(correctedImage, correctedImage, cv::ROTATE_90_COUNTERCLOCKWISE);
                    state.is_orientation_corrected = true;
                    logger.log("Image orientation corrected: 90 degrees counterclockwise rotation (EXIF 6)", LogLevel::INFO);
                    cv::imwrite(savePath, correctedImage);
                    break;
                case 7:
                    // horizontal flip + 90° CW
                    cv::flip(correctedImage, correctedImage, 1);
                    cv::rotate(correctedImage, correctedImage, cv::ROTATE_90_CLOCKWISE);
                    state.is_orientation_corrected = true;
                    logger.log("Image orientation corrected: horizontal flip + 90° CW (EXIF 7)", LogLevel::INFO);
                    cv::imwrite(savePath, correctedImage);
                    break;
                case 8:
                    // 90° CW
                    cv::rotate(correctedImage, correctedImage, cv::ROTATE_90_CLOCKWISE);
                    state.is_orientation_corrected = true;
                    logger.log("Image orientation corrected: 90 degrees clockwise rotation (EXIF 8)", LogLevel::INFO);
                    cv::imwrite(savePath, correctedImage);
                    break;
                default:
                    logger.log("Unhandled EXIF orientation: " + std::to_string(exifOrientation.value()), LogLevel::WARNING);
            }

            if (exifOrientation.value() == 3 || exifOrientation.value() == 6 || exifOrientation.value() == 8) {
                logger.log("Oriented image saved at: " + savePath, LogLevel::INFO);
            }
            return correctedImage;
        }

        logger.log("No EXIF orientation data found, applying heuristic correction", LogLevel::WARNING);

        cv::CascadeClassifier faceCascade;
        if (faceCascade.load(FACE_CASCADE_PATH)) {
            cv::Mat gray;
            cv::cvtColor(correctedImage, gray, cv::COLOR_BGR2GRAY);
            std::vector<cv::Rect> faces;
            faceCascade.detectMultiScale(gray, faces, 1.1, 3, 0, cv::Size(30, 30));

            if (!faces.empty()) {
                cv::Rect face = faces[0];
                if (face.width > face.height) {
                    cv::rotate(correctedImage, correctedImage, cv::ROTATE_90_CLOCKWISE);
                    state.is_orientation_corrected = true;
                    logger.log("Image orientation corrected via face detection (90° CW)", LogLevel::INFO);

                    std::string orientedDir = "images/temp/oriented";
                    pathutils::ensureDirectory(orientedDir);
                    std::string filename = baseName + "_oriented_face_detected.png";
                    std::string savePath = pathutils::join(orientedDir, filename);
                    cv::imwrite(savePath, correctedImage);
                    logger.log("Oriented image saved at: " + savePath, LogLevel::INFO);
                } else {
                    logger.log("Face detected with normal orientation. No rotation applied.", LogLevel::INFO);
                }
                return correctedImage;
            } else {
                logger.log("No faces detected for orientation correction.", LogLevel::INFO);
            }
        } else {
            logger.log("Could not load face cascade file for orientation correction.", LogLevel::ERROR);
        }

        // 3. Heuristic based on image dimensions
        if (correctedImage.rows > correctedImage.cols * 1.3) {
            cv::rotate(correctedImage, correctedImage, cv::ROTATE_90_CLOCKWISE);
            state.is_orientation_corrected = true;
            logger.log("Image orientation corrected via geometric heuristic (very tall image)", LogLevel::INFO);

            std::string orientedDir = "images/temp/oriented";
            pathutils::ensureDirectory(orientedDir);
            std::string filename = baseName + "_oriented_heuristic_tall.png";
            std::string savePath = pathutils::join(orientedDir, filename);
            cv::imwrite(savePath, correctedImage);
            logger.log("Oriented image saved at: " + savePath, LogLevel::INFO);
        } else {
            logger.log("General heuristic: image orientation considered correct. No rotation applied.", LogLevel::INFO);
        }

        return correctedImage;

    } catch (const std::exception& e) {
        state.error_msg = "Error correcting image orientation: " + std::string(e.what());
        logger.log(state.error_msg, LogLevel::ERROR);
        throw std::runtime_error(state.error_msg);
    }
}
