#include "Library.hpp"

Library::Library() {
    BASS_Init(-1, 44100, 0, nullptr, nullptr);
    std::string defaultPath = std::string(getenv("HOME")) + "/Music/";
    initActivePlaylist();
}

Library::~Library() {
    BASS_Free();
}

void Library::newPlaylist(std::string path) {
    _playlists[path] = Playlist(path);
    initActivePlaylist();
}

void Library::newPlaylist() {
    Playlist defPlaylist = Playlist();

    _playlists[defPlaylist.path()] = defPlaylist;
    _activePlaylist = defPlaylist.path();

    initActivePlaylist();
}

void Library::killActivePlaylist(std::string path) {
    _playlists.erase(path);
    _activePlaylist = "";
}

void Library::shuffleActivePlaylist() {
    _playlists[_activePlaylist].shuffle();
}

Playlist& Library::getActivePlaylist() {
    return _playlists[_activePlaylist];
}

int Library::playlistCount() {
    return _playlists.size();
}

void Library::initActivePlaylist() {
    _playlists[_activePlaylist].readPlaylist(); 
}
