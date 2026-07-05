#include "pathsUtils.hpp"
#include <algorithm>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

namespace pathutils {

const std::string invalidChars = "\0";  // "/" removido, apenas nulo é inválido

#ifdef _WIN32
const std::vector<std::string> reservedNames = {
    "CON", "PRN", "AUX", "NUL", "COM1", "COM2", "COM3", "COM4", "COM5",
    "COM6", "COM7", "COM8", "COM9", "LPT1", "LPT2", "LPT3", "LPT4",
    "LPT5", "LPT6", "LPT7", "LPT8", "LPT9"
};
#endif

bool isValidPath(const std::string &path) {
    if (path.empty()) return false;
    if (path.find('\0') != std::string::npos) return false; // Impede caracteres nulos 
    if (path.find("..") != std::string::npos) return false;  // Impede Path Traversal 

    // Definindo as duas bases permitidas 
    std::string datasetBase = "/home/belforz/dataset/vitalidade/candidatas_vitalidade";
    
    bool isInImages = (path.rfind("images/", 0) == 0);
    bool isInDataset = (path.rfind(datasetBase, 0) == 0);
    bool isRootFile = (path.find("/") == std::string::npos); // Arquivo local na raiz [cite: 3]

    // Se NÃO estiver em nenhum dos locais permitidos, rejeita 
    if (!isInImages && !isInDataset && !isRootFile) {
        return false;
    }

    // Checa caracteres inválidos [cite: 5]
    for (char c : path) {
        if (invalidChars.find(c) != std::string::npos)
            return false;
    }

#ifdef _WIN32
    // Checa nomes reservados do Windows
    fs::path p(path);
    for (const auto& part : p) {
        std::string upperPart = part.string();
        std::transform(upperPart.begin(), upperPart.end(), upperPart.begin(), ::toupper);
        if (std::find(reservedNames.begin(), reservedNames.end(), upperPart) != reservedNames.end())
            return false;
    }
#endif

    return true;
}

std::string sanitize(const std::string &path) {
    std::string result = path;
    // Remove caracteres inválidos
    result.erase(std::remove_if(result.begin(), result.end(),
        [](unsigned char c) {
            return invalidChars.find(c) != std::string::npos;
        }),
        result.end());
    return result;
}

std::string join(const std::string &basePath, const std::string &filename) {
    if (basePath.empty()) return filename;
    char lastChar = basePath.back();
    if (lastChar == '/' || lastChar == '\\') {
        return basePath + filename;
    } else {
        return basePath + "/" + filename;
    }
}

bool ensureDirectory(const std::string &dirPath) {
    try {
        if (dirPath.empty()) return false;
        if (fs::exists(dirPath)) {
            return fs::is_directory(dirPath);
        }
        return fs::create_directories(dirPath);
    } catch (const std::exception &e) {
        std::cerr << "Error creating directory: " << e.what() << std::endl;
        return false;
    }
}

bool fileExists(const std::string &path) {
    try {
        return fs::exists(path) && fs::is_regular_file(path);
    } catch (...) {
        return false;
    }
}

std::string getFilename(const std::string &fullPath) {
    try {
        return fs::path(fullPath).filename().string();
    } catch (...) {
        return "";
    }
}

std::string getExtension(const std::string &filename) {
    try {
        std::string ext = fs::path(filename).extension().string();
        if (!ext.empty() && ext[0] == '.') ext.erase(0, 1);
        return ext;
    } catch (...) {
        return "";
    }
}

std::string getAbsolutePath(const std::string &path) {
    try {
        return fs::absolute(path).string();
    } catch (...) {
        return path;
    }
}

std::string getFilenameWithoutExtension(const std::string &fullPath) {
    try {
        fs::path p(fullPath);
        return p.stem().string();
    } catch (...) {
        return "";
    }
}

} // namespace pathutils