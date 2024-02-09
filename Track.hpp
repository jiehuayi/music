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

        void setVolume(float volume);

        std::filesystem::path path();

        std::string name();

        double getDuration();

        double getPosition();

        std::vector<float> getChannelFFT();

    private:
        std::filesystem::path _path;
        std::string _trackPath;
        std::string _trackName;

        HSTREAM _channel;
};

