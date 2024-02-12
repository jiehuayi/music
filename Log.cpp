#include "Log.hpp"

void Log::log(const std::string& filename, const std::string& message) {
    std::ofstream file(filename);
    if (file) {
        file << message;
        file.close();
    }
}

void Log::log(const std::string& message) {
    log(DEFAULT_LOG_PATH, message);
}

void Log::append(const std::string& filename, const std::string& message) {
    std::ofstream file(filename, std::ios::app);
    if (file) {
        file << message;
        file.close();
    }
}

void Log::append(const std::string& message) {
    append(DEFAULT_LOG_PATH, message);
}

void Log::clear(const std::string& filename) {
    log(filename, "");
}

void Log::clear() {
    log(DEFAULT_LOG_PATH, "");
}
