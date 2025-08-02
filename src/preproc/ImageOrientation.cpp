#include <opencv2/opencv.hpp>
#include <optional>
#include <string>
#include <iostream>
#include "preproc/Preprocessor.hpp"
#include "Logger.hpp"

const std::string FACE_CASCADE_PATH = "/usr/share/opencv4/haarcascades/haarcascade_frontalface_default.xml";

cv::Mat correctImageOrientation(const cv::Mat& image, processorState& state, std::optional<int> exifOrientation) {
    try {
        cv::Mat correctedImage = image.clone();

        
        if (exifOrientation.has_value()) {
            switch (exifOrientation.value()) {
                case 3:
                    cv::rotate(correctedImage, correctedImage, cv::ROTATE_180);
                    state.isOrientationCorrected = true;
                    logger.log("Image orientation corrected: 180 degrees rotation", LogLevel::INFO);
                    break;
                case 6:
                    cv::rotate(correctedImage, correctedImage, cv::ROTATE_90_COUNTERCLOCKWISE);
                    state.isOrientationCorrected = true;
                    logger.log("Image orientation corrected: 90 degrees counterclockwise rotation", LogLevel::INFO);
                    break;
                case 8:
                    cv::rotate(correctedImage, correctedImage, cv::ROTATE_90_CLOCKWISE);
                    state.isOrientationCorrected = true;
                    logger.log("Image orientation corrected: 90 degrees clockwise rotation", LogLevel::INFO);
                    break;
                default:
                    logger.log("Unhandled EXIF orientation: " + std::to_string(exifOrientation.value()), LogLevel::WARNING);
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
                    // probably horizontal > turn it to vertical
                    cv::rotate(correctedImage, correctedImage, cv::ROTATE_90_CLOCKWISE);
                    state.isOrientationCorrected = true;
                    logger.log("Image orientation corrected via face detection (90° CW)", LogLevel::INFO);
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
            state.isOrientationCorrected = true;
            logger.log("Image orientation corrected via geometric heuristic (very tall image)", LogLevel::INFO);
        } else {
            logger.log("General heuristic: image orientation considered correct. No rotation applied.", LogLevel::INFO);
        }

        return correctedImage;

    } catch (const std::exception& e) {
        state.errorMsg = "Error correcting image orientation: " + std::string(e.what());
        logger.log(state.errorMsg, LogLevel::ERROR);
        throw std::runtime_error(state.errorMsg);
    }
}
