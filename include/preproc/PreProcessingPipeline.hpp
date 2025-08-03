#ifndef PREPROCESSING_PIPELINE_HPP
#define PREPROCESSING_PIPELINE_HPP

#include <opencv2/opencv.hpp>
#include "Metadata.hpp"
#include "ImageHasher.hpp"
#include <optional>

/**
 * @brief Runs the full preprocessing pipeline on an image.
 * 
 * @param imagePath Path to the image file.
 * @return Processed ImageMetadata with status flags and diagnostics.
 * @throws std::runtime_error in case of failure during any step.
 */
ImageMetadata runPreprocessingPipeline(const std::string &imagePath, std::optional<int> exifOrientation = std::nullopt);

#endif // PREPROCESSING_PIPELINE_HPP
