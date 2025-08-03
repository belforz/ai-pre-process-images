#include <opencv2/opencv.hpp>
#include "preproc/Preprocessor.hpp"
#include "Logger.hpp"
#include <optional>
#include <vector>
#include <string>
#include <iostream>

void testNormalization() {
    logger.log("NORMALIZAÇÃO DE IMAGENS", LogLevel::SECTION);

    std::vector<std::pair<std::string, int>> imagesNormalize = {
        {"./images/img_8bit.png", 1},
        {"./images/img_16bit.tiff", 6},
        {"./images/img_32float_raw.npy", 3},
        {"./images/img_alert_saturation.png", 8},
    };

    for (size_t i = 0; i < imagesNormalize.size(); ++i) {
        const auto& path = imagesNormalize[i].first;
        cv::Mat image = cv::imread(path);

        std::cout << "Testing normalization for: " << path << std::endl;

        if (image.empty()) {
            std::cerr << "  Failed to load image." << std::endl;
            continue;
        }

        processorState state;
        cv::Mat normalized = normalizeImage(image, state);

        std::string outName = "./images/temp/normalized_" + std::to_string(i+1) + ".jpg";
        cv::imwrite(outName, normalized);
        logger.log("Image saved to: " + outName, LogLevel::INFO);

        std::cout << "  Processed -> " << outName
                  << " | Saturated: " << state.saturated
                  << " | Underexposed: " << state.underexposed << std::endl;
    }
}

void testCompression() {
    logger.log("CHECAGEM DE COMPRESSÃO", LogLevel::SECTION);

    std::vector<std::pair<std::string, std::string>> imagesCompressions = {
        {"./images/img_compressed_jpeg.jpg", "jpeg"},
        {"./images/img_compressed_webp.webp", "webp"},
        {"./images/img_uncompressed.png", "png"},
    };

    for(size_t i = 0; i < imagesCompressions.size(); ++i){
        const auto& path = imagesCompressions[i].first;
        cv::Mat image = cv::imread(path);

        std::cout << "Testing compression for: " << path << std::endl;

        if (image.empty()) {
            std::cerr << "  Failed to load image." << std::endl;
            continue;
        }

        processorState state;
        IsCompressed(image, state, path);
        logger.log("Compression check for " + path + ": " + (state.hasCompressedImage ? "Compressed" : "Uncompressed"), LogLevel::INFO);
        std::cout << "  Compression check: "
                  << (state.hasCompressedImage ? "Compressed" : "Uncompressed") << std::endl;
    }
}

void testFloatNormalization() {
    logger.log("TESTE DE NORMALIZAÇÃO DE FLOAT", LogLevel::SECTION);

    cv::Mat floatImg = cv::Mat::ones(100, 100, CV_32F) * 123.45f;
    cv::randu(floatImg, 0.0f, 255.0f);

    processorState state;
    cv::Mat norm = normalizeImage(floatImg, state);

    cv::imwrite("./images/temp/test_float_to_8bit.png", norm);
    logger.log("Float image normalized and saved to ./images/temp/test_float_to_8bit.png", LogLevel::INFO);
}

int main() {
    testNormalization();
    testCompression();
    testFloatNormalization();
    return 0;
}