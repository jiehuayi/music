#include <filesystem>
#include <vector>

class Directory {
    public:
        Directory();
        ~Directory();
        
        void clearSelectedDirectory();

        std::string queryNextSubdirectory();
        std::string getDirectoryName();
        std::filesystem::path parseDirectoryFromString();
    private:
        std::filesystem::path _selectedDirectory;
        std::vector<std::filesystem::path> _subdirectories;
        unsigned int _subdirectoryQueryIndex;
};
