#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <filesystem>

#define STD_MPATH "/home/jason/Music"

#define EXT_OGG ".ogg"
#define EXT_WAV ".wav"
#define EXT_MP3 ".mp3"

#define READ_DIR_SUCCESS 		0
#define READ_DIR_ERROR			1

class Playlist {

public:

  Playlist();

  Playlist(std::string path);
  
  ~Playlist();

  int readPlaylist();

  std::vector<std::string> getPlaylistSongs();

private:

  std::string _path;

  std::vector<std::filesystem::path> _songs; 
  
};
