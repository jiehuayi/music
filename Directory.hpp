#include <filesystem>
#include <string>
#include <vector>

#define PATH_IDENTIFIER '\''

class Directory {
    public:
        Directory();
        ~Directory();
        
        void clearSelectedDirectory();

        std::string queryNextSubdirectory();
        std::string getDirectoryName();
        std::filesystem::path parseDirectoryFromString(std::string str);
    private:
        std::filesystem::path _selectedDirectory;
        std::vector<std::filesystem::path> _subdirectories;
        unsigned int _subdirectoryQueryIndex;
};
