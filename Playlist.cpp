#include "Playlist.hpp"

Playlist::Playlist() {
  _songs = {};
  _path = std::string(getenv("HOME")) + "/Music/";
}

Playlist::Playlist(std::string path) {
  _songs = {};
  _path = path;
}

Playlist::~Playlist() {}

int Playlist::readPlaylist() {
  try {
    if (std::filesystem::exists(_path) && std::filesystem::is_directory(_path)) {
      for (const auto& entry : std::filesystem::directory_iterator(_path)) {
	std::filesystem::path ext = entry.path().extension();
	std::string extension = ext.u8string();
	if (extension == ".ogg" || extension == ".wav" || extension == ".mp3") {
	  _songs.push_back(entry.path());
	}
      }
    }

  } catch (const std::exception& e) {
    std::cerr << "EXCEPTION: " << e.what() << std::endl;
  }

  return READ_DIR_SUCCESS;
}

std::vector<std::string> Playlist::getPlaylistSongs() {
  std::vector<std::string> ret = {};
  
  for (const auto& path : _songs) {
    ret.push_back(path.filename().string());
  }

  return ret;
}

void play(int index) {
  
}




