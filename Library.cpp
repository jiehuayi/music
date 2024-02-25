#include "Library.hpp"

Library::Library() {
    BASS_Init(-1, 44100, 0, nullptr, nullptr);
    newPlaylist();
    newPlaylist(std::string(getenv("HOME")) + "/Music/in/");
}

Library::~Library() {
    BASS_Free();
}

void Library::newPlaylist(std::string path) {
    _playlists[path] = Playlist(path);
    _activePlaylist = path;
    initActivePlaylist();
}

void Library::newPlaylist() {
    Playlist defPlaylist = Playlist();

    _playlists[defPlaylist.path()] = defPlaylist;
    _activePlaylist = defPlaylist.path();

    initActivePlaylist();
}

void Library::circulateActivePlaylist() {
    auto playlistIt = _playlists.find(_activePlaylist);
    playlistIt++;

    if (playlistIt == _playlists.end()) {
        _activePlaylist = _playlists.begin()->first;
    } else {
        _activePlaylist = playlistIt->first;
    }
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
