#include <nlohmann/json.hpp>
using json = nlohmann::json;
#include <string>

#ifndef IMAGE_METADATA_HPP
#define IMAGE_METADATA_HPP


struct PreprocessorState
{
    // Status flags
    bool is_image_valid = false;
    bool is_image_resized = false;
    bool is_resolution_critic = false;
    bool has_exif_data = false;
    bool is_orientation_corrected = false;
    bool has_compressed_image = false;
    bool thumbnail_generated = false;
    // bool blur_generated = false;
    bool color_spaces_generated = false;
    bool is_normalized = false;
    bool has_satured_pixels = false;
    bool has_underexposed_pixels = false;

    std::string original_filename;

    // Diagnostics and messages
    std::string error_msg;

    // Compression diagnostics
    std::string compression_type;

    // Orientation diagnostics
    std::string orientation_method;

    // saturated pixels
    int saturated_pixel_count = 0;
    int underexposed_pixel_count = 0;

    // Blur diagnostics
    // std::string blur_type;
};
using processorState = PreprocessorState;

/**
 * @brief Represents metadata associated with an image.
 *
 * This structure holds information about the image such as filename, hash, dimensions,
 * aspect ratio, original format, and type. It also includes flags indicating the state
 * of preprocessing.
 */

struct ImageMetadata
{
    std::string filename;
    std::string hash; // SHA-256
    int width = 0;
    int height = 0;
    double aspect_ratio = 0.0;
    std::string original_format;
    std::string image_type; // e.g., "photo", "scan", "document", etc.

    // Additional fields
    std::string color_space;      // e.g., "BGR", "RGB", "GRAY", etc.
    std::string exif_orientation; // Original EXIF value, if available
    std::string source;           // e.g., "upload", "camera", "web", etc.

    PreprocessorState preproc_state; // Detailed preprocessing status
};

#endif // IMAGE_METADATA_HPP
