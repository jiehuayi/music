#pragma once

#include <iostream>
#include <string>
#include <memory>
#include <map>
#include <bass.h>

#include "Playlist.hpp"

class PlaylistManager {
    public:
        PlaylistManager();
        ~PlaylistManager();

        void newPlaylist(std::string path);
        void newPlaylist();
        void nextPlaylist();
        void killPlaylist(std::string path);

        void shuffleActivePlaylist();
        Playlist& getDisplayPlaylist();

        int playlistCount();

    private:
        void initDisplayPlaylist();

    private:
        std::string _displayPlaylist;
        std::map<std::string, Playlist> _playlists;
};
