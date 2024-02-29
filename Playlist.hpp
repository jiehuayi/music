#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <algorithm>
#include <random>
#include <cmath>
#include <unistd.h>

#include "Track.hpp"

#define EXT_OGG ".ogg"
#define EXT_WAV ".wav"
#define EXT_MP3 ".mp3"

#define READ_DIR_SUCCESS 		0
#define READ_DIR_ERROR			1

#define VOLUME_STEP			    0.05

class Playlist {
    public:
        bool operator==(const Playlist& other) const;
        bool operator!=(const Playlist& other) const;

        Playlist();
        Playlist(std::string path);
        ~Playlist();

        int readPlaylist();
        int getSize();
        std::string getPath();
        std::vector<std::string> getPlaylistSongs();
        std::vector<float> getFFT();
        double getPosition();
        double getDuration();
        std::string activeSongName();
        bool isPlaying();
        bool isFinished();

        void incVolume();
        void decVolume();
        void setVolume(float volume);
        float getVolume();

        void play(int index);
        void trigger();
        void shuffle();
    private:
        void init();
    private:
        float _volume;
        std::string _path;
        bool _isTrackPlaying;
        std::shared_ptr<Track> _activeTrack;

        std::vector<std::filesystem::path> _songs;
        std::vector<std::filesystem::path> _songsBase;
};
