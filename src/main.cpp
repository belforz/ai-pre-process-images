#include <opencv2/opencv.hpp>
#include "preproc/PreProcessingPipeline.hpp"
#include "Logger.hpp"
#include <string>
#include <iostream>
#include <nlohmann/json.hpp>
#include "preproc/Metadata.hpp"
#include "preproc/MetadataSerializer.hpp"
#include <vector>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <img1> [exif1] <img2> [exif2] ..." << std::endl;
        return 1;
    }

    for (int i = 1; i < argc; ) {
        std::string imagePath = argv[i];
        std::optional<int> exifOrientation = std::nullopt;
        if (i + 1 < argc && std::isdigit(argv[i + 1][0])) {
            exifOrientation = std::stoi(argv[i + 1]);
            i += 2;
        } else {
            i += 1;
        }
        std::cout << "\n=== Processing: " << imagePath;
        if (exifOrientation) std::cout << " (EXIF: " << *exifOrientation << ")";
        std::cout << " ===\n";
        try {
            ImageMetadata metadata = runPreprocessingPipeline(imagePath, exifOrientation);
            nlohmann::json j = metadata;
            std::cout << "Pipeline result:\n" << j.dump(4) << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Error processing image " << imagePath << ": " << e.what() << std::endl;
        }
    }
    return 0;
}