#pragma once

#include <iostream>
#include <filesystem>
#include <string>
#include <cstring>

#include <bass.h>

#define BUFF_SZ 2048

class Track {
    public:
        Track(std::filesystem::path path);
        ~Track();

        void play();
        void pause();

        bool isEnd();

        std::filesystem::path path();
        std::string name();
        void setVolume(float volume);
        double getDuration();
        double getPosition();
        std::vector<float> getChannelFFT();

    private:
        HSTREAM _channel;
        std::filesystem::path _path;
        std::string _trackPath;
        std::string _trackName;
};

