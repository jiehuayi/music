#include <filesystem>

class Directory {
    public:
        Directory();
        ~Directory();
        
        void clearSelectedDirectory();

        std::string queryNextSubdirectory();
        std::string getDirectoryName();
    private:
        std::filesystem::path selectedDirectory();
};
