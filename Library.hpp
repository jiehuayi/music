#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include <bass.h>

#include "Playlist.hpp"

class Library {
public:
  Library();
  ~Library();
  
  void newPlaylist(std::string path);
  void newPlaylist();
  void killPlaylist(std::string path);
  void killActivePlaylist();
  Playlist& getActivePlaylist();
  
  int playlistCount();

private:
  void initActivePlaylist();
  
private:
  std::string _activePlaylist;
  std::unordered_map<std::string, Playlist> _playlists;
};
