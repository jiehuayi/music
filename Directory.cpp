#include "Directory.hpp"

Directory::Directory() {}

Directory::~Directory() {}

void Directory::clearSelectedDirectory() {
    _selectedDirectory = "";
    _subdirectories = {};
    _subdirectoryQueryIndex = 0;
}

std::string Directory::queryNextSubdirectory() {
    std::filesystem::path next = _subdirectories[_subdirectoryQueryIndex];
    _subdirectoryQueryIndex = (_subdirectoryQueryIndex++) % _subdirectories.size();
    return next.string();
}

std::string Directory::getDirectoryName() {
    return _selectedDirectory.string();
}

std::filesystem::path Directory::parseDirectoryFromString(std::string str) {
    size_t quoteStart = str.find(PATH_IDENTIFIER);
    if (quoteStart == std::string::npos) {
        return std::filesystem::path("");
    }

    size_t quoteEnd = str.rfind(PATH_IDENTIFIER);
    std::filesystem::path extractedPath;

    if (quoteStart != quoteEnd) {
        extractedPath = str.substr(quoteStart, quoteEnd - quoteStart);
    } else {
        extractedPath = str.substr(quoteStart);
    }
    
    _selectedDirectory = extractedPath.parent_path();
    return extractedPath;
}
