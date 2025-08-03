#include "preproc/MetadataSerializer.hpp"
#include "pathsUtils.hpp"
#include <fstream>
#include <stdexcept>

void saveMetadataAsJson(const ImageMetadata &metadata, const std::string &outputPath)
{
    
    std::string filename = pathutils::getFilename(outputPath);
    std::string safePath = "local/json/" + filename;

    
    if (!pathutils::ensureDirectory("local/json")) {
        throw std::runtime_error("Failed to create directory for metadata: local/json");
    }

    nlohmann::json j = metadata;

    std::ofstream file(safePath);
    if (!file)
    {
        throw std::runtime_error("Failed to write metadata to: " + safePath);
    }
    file << j.dump(4);
}