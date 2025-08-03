#ifndef IMAGE_METADATA_SERIALIZER_HPP
#define IMAGE_METADATA_SERIALIZER_HPP

#include "Metadata.hpp"
#include <nlohmann/json.hpp>

inline void to_json(nlohmann::json &j, const PreprocessorState &p)
{
    j = {
        {"is_image_valid", p.is_image_valid},
        {"is_image_resized", p.is_image_resized},
        {"is_resolution_critic", p.is_resolution_critic},
        {"has_exif_data", p.has_exif_data},
        {"is_orientation_corrected", p.is_orientation_corrected},
        {"has_compressed_image", p.has_compressed_image},
        {"thumbnail_generated", p.thumbnail_generated},
        {"color_spaces_generated", p.color_spaces_generated},
        {"is_normalized", p.is_normalized},
        {"original_filename", p.original_filename},

        // Diagnostics
        {"saturated", p.saturated},
        {"underexposed", p.underexposed},
        {"error_msg", p.error_msg},
        {"compression_type", p.compression_type},
        {"orientation_method", p.orientation_method}
        // {"blur_type", p.blur_type} // add in the future
    };
}

inline void from_json(const nlohmann::json &j, PreprocessorState &p)
{
    j.at("is_image_valid").get_to(p.is_image_valid);
    j.at("is_image_resized").get_to(p.is_image_resized);
    j.at("is_resolution_critic").get_to(p.is_resolution_critic);
    j.at("has_exif_data").get_to(p.has_exif_data);
    j.at("is_orientation_corrected").get_to(p.is_orientation_corrected);
    j.at("has_compressed_image").get_to(p.has_compressed_image);
    j.at("thumbnail_generated").get_to(p.thumbnail_generated);
    j.at("color_spaces_generated").get_to(p.color_spaces_generated);
    j.at("is_normalized").get_to(p.is_normalized);
    j.at("original_filename").get_to(p.original_filename);

    // Diagnostics
    j.at("saturated").get_to(p.saturated);
    j.at("underexposed").get_to(p.underexposed);
    j.at("error_msg").get_to(p.error_msg);
    j.at("compression_type").get_to(p.compression_type);
    j.at("orientation_method").get_to(p.orientation_method);
    // j.at("blur_type").get_to(p.blur_type); // add in the future
}

inline void to_json(nlohmann::json &j, const ImageMetadata &m)
{
    j = {
        {"filename", m.filename},
        {"hash", m.hash},
        {"dimensions", {m.width, m.height}},
        {"aspect_ratio", m.aspect_ratio},
        {"original_format", m.original_format},
        {"image_type", m.image_type},
        {"color_space", m.color_space},
        {"exif_orientation", m.exif_orientation},
        {"source", m.source},
        {"preproc_state", m.preproc_state}
    };
}

inline void from_json(const nlohmann::json &j, ImageMetadata &m)
{
    j.at("filename").get_to(m.filename);
    j.at("hash").get_to(m.hash);
    const auto &dims = j.at("dimensions");
    m.width = dims.at(0).get<int>();
    m.height = dims.at(1).get<int>();
    j.at("aspect_ratio").get_to(m.aspect_ratio);
    j.at("original_format").get_to(m.original_format);
    j.at("image_type").get_to(m.image_type);
    j.at("color_space").get_to(m.color_space);
    j.at("exif_orientation").get_to(m.exif_orientation);
    j.at("source").get_to(m.source);
    j.at("preproc_state").get_to(m.preproc_state);
};

void saveMetadataAsJson(const ImageMetadata &metadata, const std::string &outputPath);

#endif // IMAGE_METADATA_SERIALIZER_HPP
