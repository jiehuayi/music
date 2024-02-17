#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <cmath>
#include <unistd.h>

#include "Track.hpp"

#undef KEY_ENTER
#undef KEY_BACKSPACE
#undef KEY_UP
#undef KEY_DOWN
#undef KEY_RIGHT
#undef KEY_LEFT
#undef KEY_HOME
#undef KEY_END

#define EXT_OGG ".ogg"
#define EXT_WAV ".wav"
#define EXT_MP3 ".mp3"

#define READ_DIR_SUCCESS 		0
#define READ_DIR_ERROR			1

#define VOLUME_STEP			0.05

class Playlist {
    public:
        Playlist();
        Playlist(std::string path);
        ~Playlist();

        int readPlaylist();
        int size();

        std::string path();

        void incVolume();
        void decVolume();
        void setVolume(float volume);
        float getVolume();

        std::vector<std::string> getPlaylistSongs();

        void play(int index);
        void trigger();

        std::string activeSongName();
        bool isPlaying();

        std::vector<float> getFFT();
        double getDuration();
        double getPosition();

    private:
        float _volume;
        std::string _path;
        bool _isTrackPlaying;
        std::vector<std::filesystem::path> _songs;
        std::shared_ptr<Track> _activeTrack;

};
