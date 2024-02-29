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
        void nextPlaylist();
        void killPlaylist(std::string path);

        void shuffleActivePlaylist();
        Playlist& getDisplayPlaylist();
        Playlist& getActivePlaylist();
        void setDisplayAsActive();

        int playlistCount();

    private:
        void initDisplayPlaylist();

        Playlist& getPlaylist(std::string identifier);

    private:
        std::string _displayPlaylist;
        std::string _activePlaylist;
        std::map<std::string, Playlist> _playlists;
};
