// --------------- for debugging purposes ---------------

#pragma once

#include <fstream>

namespace Log {
    void log(const std::string& filename, const std::string& message);
    void log(const std::string& message);
    void append(const std::string& filename, const std::string& message);
    void append(const std::string& message);
    void clear(const std::string& filename);
    void clear();

    const std::string DEFAULT_LOG_PATH = "./Penelope.log";
};
