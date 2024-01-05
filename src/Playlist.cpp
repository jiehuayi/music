#include "Playlist.hpp"

Playlist::Playlist() {
  _root = _DEFAULT_ROOT;
}

Playlist::Playlist(string root) {
  _root = root;
}

Playlist::~Playlist() {}

readPlaylist() {
  for (const auto& entry: fs::directory_iterator(_root)) {
	std::cout << entry.path().string();
	_songs.push_back(entry.path().string());
  }
}
