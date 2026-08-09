#include <opencv2/opencv.hpp>
#include "preproc/PreProcessingPipeline.hpp"
#include "Logger.hpp"
#include <string>
#include <iostream>
#include <nlohmann/json.hpp>
#include "preproc/Metadata.hpp"
#include "preproc/MetadataSerializer.hpp"
#include <vector>
#include <fstream>

namespace {

std::optional<nlohmann::json> parsePayloadArg(const std::string& payloadArg) {
    try {
        // Accept raw JSON payload directly in argv[1].
        return nlohmann::json::parse(payloadArg);
    } catch (const std::exception&) {
        // If parsing fails, try loading a JSON file path.
    }

    std::ifstream in(payloadArg);
    if (!in.is_open()) {
        return std::nullopt;
    }

    try {
        nlohmann::json payload;
        in >> payload;
        return payload;
    } catch (const std::exception&) {
        return std::nullopt;
    }
}

bool parseExifArg(const char* value, int& exifOut) {
    try {
        std::string candidate(value);
        size_t pos = 0;
        int parsed = std::stoi(candidate, &pos);
        if (pos != candidate.size()) {
            return false;
        }
        exifOut = parsed;
        return true;
    } catch (const std::exception&) {
        return false;
    }
}

// Pulls a "--category <name>" pair out of the manual-mode argument list, if present,
// so it isn't mistaken for an image path or EXIF value.
std::string extractCategoryFlag(std::vector<std::string>& args) {
    std::string category;
    for (size_t i = 0; i < args.size(); ++i) {
        if (args[i] == "--category" && i + 1 < args.size()) {
            category = args[i + 1];
            args.erase(args.begin() + static_cast<long>(i), args.begin() + static_cast<long>(i) + 2);
            break;
        }
    }
    return category;
}

}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " [--category <name>] <img1> [exif1] <img2> [exif2] ...\n"
                  << "   or: " << argv[0] << " <payload-json-or-file> <img1> [exif1] <img2> [exif2] ...\n"
                  << "\n"
                  << "<img*> pode apontar para qualquer arquivo dentro de images/, do dataset local,\n"
                  << "ou da pasta dinamica de uploads /home/belforz/photus-system/data/uploads/{uuid}_{YYYYMMDD_HHMMSS}/nome_da_foto.formato."
                  << std::endl;
        return 1;
    }

    std::vector<std::string> args(argv + 1, argv + argc);
    std::string category;

    if (auto payloadOpt = parsePayloadArg(args[0]); payloadOpt.has_value()) {
        const nlohmann::json& payload = payloadOpt.value();
        const bool hasCategoryCode = payload.contains("category_code") && payload["category_code"].is_string() && !payload["category_code"].get<std::string>().empty();

        if (!hasCategoryCode) {
            std::cerr << "Payload recebido sem 'category_code'. Trigger do pre-processamento nao foi acionado." << std::endl;
            return 1;
        }

        category = payload["category_code"].get<std::string>();
        std::cout << "Trigger de pre-processamento acionado por category_code='" << category << "'" << std::endl;
        args.erase(args.begin());
    } else {
        // Modo manual (fora do trigger de IA): aceita uma categoria explicita via --category,
        // usada para marcar as imagens processadas no images_index.json.
        category = extractCategoryFlag(args);
    }

    if (args.empty()) {
        std::cerr << "Nenhuma imagem informada." << std::endl;
        return 1;
    }

    for (size_t i = 0; i < args.size(); ) {
        std::string imagePath = args[i];
        std::optional<int> exifOrientation = std::nullopt;
        int parsedExif = 1;
        if (i + 1 < args.size() && parseExifArg(args[i + 1].c_str(), parsedExif)) {
            exifOrientation = parsedExif;
            i += 2;
        } else {
            i += 1;
        }
        std::cout << "\n=== Processing: " << imagePath;
        if (exifOrientation) std::cout << " (EXIF: " << *exifOrientation << ")";
        if (!category.empty()) std::cout << " (category: " << category << ")";
        std::cout << " ===\n";
        try {
            ImageMetadata metadata = runPreprocessingPipeline(imagePath, exifOrientation, category);
            nlohmann::json j = metadata;
            std::cout << "Pipeline result:\n" << j.dump(4) << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Error processing image " << imagePath << ": " << e.what() << std::endl;
        }
    }
    return 0;
}