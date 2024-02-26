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
    _displayPlaylist = path;
    initDisplayPlaylist();
}

void Library::newPlaylist() {
    Playlist defPlaylist = Playlist();

    _playlists[defPlaylist.getPath()] = defPlaylist;
    _displayPlaylist = defPlaylist.getPath();

    initDisplayPlaylist();
}

void Library::nextPlaylist() {
    auto playlistIt = _playlists.find(_displayPlaylist);
    playlistIt++;

    if (playlistIt == _playlists.end()) {
        _displayPlaylist = _playlists.begin()->first;
    } else {
        _displayPlaylist = playlistIt->first;
    }
}

void Library::killPlaylist(std::string path) {
    _playlists.erase(path);
    _displayPlaylist = "";
}

void Library::shuffleActivePlaylist() {
    _playlists[_displayPlaylist].shuffle();
}

Playlist& Library::getDisplayPlaylist() {
    return _playlists[_displayPlaylist];
}

int Library::playlistCount() {
    return _playlists.size();
}

void Library::initDisplayPlaylist() {
    _playlists[_displayPlaylist].readPlaylist(); 
}
