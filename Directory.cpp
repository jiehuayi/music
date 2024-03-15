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

std::filesystem::path Directory::parseDirectoryFromString() {
    
}
