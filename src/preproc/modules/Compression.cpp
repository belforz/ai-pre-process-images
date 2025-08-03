#include <opencv2/opencv.hpp>
#include "preproc/Preprocessor.hpp"
#include "preproc/Metadata.hpp"
#include "Logger.hpp"
#include <optional>
#include <fstream>
#include <algorithm>
#include <filesystem>


void IsCompressed(const cv::Mat& image, processorState& state, const std::string& imagePathOrExt) {
    try {
        if (image.empty()) {
            state.error_msg = "Input image is empty.";
            logger.log(state.error_msg, LogLevel::ERROR);
            return;
        }

        std::string ext = std::filesystem::path(imagePathOrExt).extension().string();
        std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

        // Verifiy valid extensions
        if (ext == ".jpg" || ext == ".jpeg" || ext == ".webp") {
            state.has_compressed_image = true;
            logger.log("Compression detected via file extension: " + ext, LogLevel::INFO);
            return;
        }

        // Verify binary header
        std::ifstream file(imagePathOrExt, std::ios::binary);
        if (file) {
            unsigned char buffer[12] = {0};
            file.read(reinterpret_cast<char*>(buffer), sizeof(buffer));
            file.close();

            // JPEG: FF D8 FF
            if (buffer[0] == 0xFF && buffer[1] == 0xD8 && buffer[2] == 0xFF) {
                state.has_compressed_image = true;
                state.compression_type = "JPEG";
                logger.log("Compression detected via JPEG signature.", LogLevel::INFO);
                return ;
            }

            // WEBP: RIFF....WEBP
            if (buffer[0] == 'R' && buffer[1] == 'I' && buffer[2] == 'F' && buffer[3] == 'F'
                && buffer[8] == 'W' && buffer[9] == 'E' && buffer[10] == 'B' && buffer[11] == 'P') {
                state.has_compressed_image = true;
                state.compression_type = "WEBP";
                logger.log("Compression detected via WEBP signature.", LogLevel::INFO);
                return;
            }
        }

        state.has_compressed_image = false;
        logger.log("No compression detected in image.", LogLevel::INFO);
        return ;

    } catch (const std::exception& e) {
        state.error_msg = "Error checking compression: " + std::string(e.what());
        logger.log(state.error_msg, LogLevel::ERROR);
        return;
    }
}
