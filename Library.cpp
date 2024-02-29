#include "Library.hpp"

PlaylistManager::PlaylistManager() {
    BASS_Init(-1, 44100, 0, nullptr, nullptr);
    Playlist defPlaylist = Playlist();

    _playlists[defPlaylist.getPath()] = defPlaylist;
    _displayPlaylist = defPlaylist.getPath();
    _activePlaylist = _displayPlaylist;
    initDisplayPlaylist();
    
    newPlaylist(_activePlaylist);
    newPlaylist(_activePlaylist + "/in/");
}

PlaylistManager::~PlaylistManager() {
    BASS_Free();
}

void PlaylistManager::newPlaylist(std::string path) {
    _playlists[path] = Playlist(path);
    _displayPlaylist = path;
    initDisplayPlaylist();
}

void PlaylistManager::nextPlaylist() {
    auto playlistIt = _playlists.find(_displayPlaylist);
    playlistIt++;

    if (playlistIt == _playlists.end()) {
        _displayPlaylist = _playlists.begin()->first;
    } else {
        _displayPlaylist = playlistIt->first;
    }
}

void PlaylistManager::killPlaylist(std::string path) {
    _playlists.erase(path);
    _displayPlaylist = "";
}

void PlaylistManager::shuffleActivePlaylist() {
    _playlists[_displayPlaylist].shuffle();
}

Playlist& PlaylistManager::getDisplayPlaylist() {
    return getPlaylist(_displayPlaylist);
}

Playlist& PlaylistManager::getActivePlaylist() {
    return getPlaylist(_activePlaylist);
}

Playlist& PlaylistManager::getPlaylist(std::string identifier) {
    return _playlists[identifier];
}

void PlaylistManager::setDisplayAsActive() {
    _activePlaylist = _displayPlaylist;
}

int PlaylistManager::playlistCount() {
    return _playlists.size();
}

void PlaylistManager::initDisplayPlaylist() {
    _playlists[_displayPlaylist].readPlaylist(); 
}
