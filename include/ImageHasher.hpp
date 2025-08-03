#ifndef IMAGE_HASHER_HPP
#define IMAGE_HASHER_HPP

#include <string>

/**
 * @brief Computes the SHA-256 hash of a file (binary mode).
 *
 * @param filepath Path to the image or file.
 * @return Hexadecimal string of the SHA-256 hash.
 */
std::string computeSHA256(const std::string &filepath);

#endif // IMAGE_HASHER_HPP
