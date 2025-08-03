#ifndef PATHUTILS_HPP
#define PATHUTILS_HPP

#include <string>

namespace pathutils {

    /**
     * Joins two paths correctly using appropriate separators.
     * @param base Base path.
     * @param child Child path to be appended.
     * @return Combined path.
     */
    std::string join(const std::string &base, const std::string &child);

    /**
     * Removes dangerous elements like ../ to prevent path traversal.
     * @param path Path to be sanitized.
     * @return Safe path.
     */
    std::string sanitize(const std::string &path);

    /**
     * Checks if the path is "reasonable" and does not contain dangerous elements.
     * @param path Path to be checked.
     * @return true if the path is valid, false otherwise.
     */
    bool isValidPath(const std::string &path);

    /**
     * Checks if the file exists.
     * @param path File path.
     * @return true if the file exists, false otherwise.
     */
    bool fileExists(const std::string &path);

    /**
     * Creates directory if it does not exist.
     * @param path Directory path.
     * @return true if the directory exists or was created successfully, false otherwise.
     */
    bool ensureDirectory(const std::string &path);

    /**
     * Extracts the filename from a full path.
     * @param fullPath Full file path.
     * @return Filename.
     */
    std::string getFilename(const std::string &fullPath);

    /**
     * Extracts the filename without extension from a full path.
     * @param fullPath Full file path.
     * @return Filename without extension.
     */
    std::string getFilenameWithoutExtension(const std::string &fullPath);

    /**
     * Gets the extension of a file (without the dot).
     * @param filename File name.
     * @return File extension.
     */
    std::string getExtension(const std::string &filename);

    /**
     * Resolves the absolute path (platform-dependent).
     * @param path Relative or absolute path.
     * @return Absolute path.
     */
    std::string getAbsolutePath(const std::string &path);

}

#endif // PATHUTILS_HPP
