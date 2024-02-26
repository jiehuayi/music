#pragma once

#include <fstream>
#include <sstream>
#include <string>

#define DEFAULT_LOG "PENELOPE.log"

#define TRACE_PREFIX(LevelNumber) ("TRACE[" + std::string(LevelNumber) + "] ")

#define META(LEVEL_NUM) \
    ([]() { \
        auto now = std::chrono::system_clock::now(); \
        auto now_c = std::chrono::system_clock::to_time_t(now); \
        std::ostringstream oss; \
        oss << TRACE_PREFIX(LEVEL_NUM) \
            << "[FILE: " << __FILE__ \
            << "] [LINE: " << __LINE__ \
            << "] "; \
        std::ostringstream timestamp; \
        timestamp << "[" << std::put_time(std::localtime(&now_c), "%F %T") << "] "; \
        return timestamp.str() + oss.str(); \
    })() + "[FUNCTION: " + __FUNCTION__ + "] "

#define LOG(MESSAGE) \
    do { \
        std::ofstream file(DEFAULT_LOG, std::ios::app); \
        if (file) { \
            file << MESSAGE << "\n"; \
        } \
    } while (0)

#define TRACEN(LEVEL_NUM, MESSAGE) \
    LOG(META(LEVEL_NUM) + MESSAGE)

#define TRACE1(MESSAGE) \
    TRACEN("1", MESSAGE)

#define TRACE2(MESSAGE) \
    TRACEN("2", MESSAGE)

#define TRACE3(MESSAGE) \
    TRACEN("3", MESSAGE)

#define TRACE_INIT() \
    do { \
        std::ofstream file(DEFAULT_LOG, std::ofstream::out | std::ofstream::trunc); \
        if (file.is_open()) { \
            file.close(); \
        } \
    } while (0)

