#include "Clock.hpp"

Clock::Clock() {}

Clock::~Clock() {}

void Clock::start() {
    _clock = std::chrono::high_resolution_clock::now();
}

double Clock::elapsed() {
    std::chrono::time_point<std::chrono::high_resolution_clock> check =
        std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double, std::milli>(check - _clock).count();
}

bool Clock::hasElapsed(double ms) {
    return elapsed() >= ms;
}
