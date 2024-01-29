#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <filesystem>

#include <SDL2/SDL.h>

#include <unistd.h>

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


class Playlist {

public:

  Playlist();

  Playlist(std::string path);
  
  ~Playlist();

  int readPlaylist();

  std::vector<std::string> getPlaylistSongs();

  void play(int index);

private:

  std::string _path;

  std::vector<std::filesystem::path> _songs;
  
};
