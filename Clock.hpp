#pragma once

#include <chrono>

class Clock {
    public:
        Clock();
        ~Clock();
        
        void start();
        double elapsed();
        bool hasElapsed(double ms);

    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> _clock;
};
