#include "Playlist.hpp"

Playlist::Playlist() {
  BOOL bassInit = BASS_Init(-1, 44100, 0, 0, NULL);
  
  if (!bassInit) {
    std::cerr
      << "ERROR: Failed to init BASS library."
      << std::endl;
  }
  
  _songs = {};
  _path = std::string(getenv("HOME")) + "/Music/";
  _activeTrack = nullptr;
  _isTrackPlaying = false;
}

Playlist::Playlist(std::string path) {
  _songs = {};
  _path = path;
  _activeTrack = nullptr;
  _isTrackPlaying = false;
}

Playlist::~Playlist() {
  BASS_Free();
}

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

int Playlist::size() {
  return _songs.size();
}


void Playlist::play(int index) {
  std::filesystem::path selectedTrack = _songs[index];

  _isTrackPlaying = true;
  
  if (_activeTrack == nullptr || selectedTrack != _activeTrack->path()) {
    _activeTrack = std::make_unique<Track>(selectedTrack);
    _activeTrack->play();
  }  
}

void Playlist::trigger() {
  if (!_activeTrack) return;
  
  if (_isTrackPlaying) {
    _activeTrack->pause();
  } else {
    _activeTrack->play();
  }

  _isTrackPlaying = !_isTrackPlaying;
}

double Playlist::progress() {
  if (!_activeTrack) {
    return 0.0;
  }
  
  double pos = _activeTrack->getPosition();
  double dur = _activeTrack->getDuration();

  if (pos < 0 || dur < 0) {
    return 0.0;
  }
  
  return pos / dur;
}

std::string Playlist::activeSongName() {
  if (_activeTrack) {
    return _activeTrack->name();
  }

  return "?";
}
