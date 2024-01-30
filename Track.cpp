#include "Track.hpp"

Track::Track(std::string path) : _path(std::move(path)) {
  BOOL bassInit = BASS_Init(-1, 44100, 0, 0, NULL);
  
  if (!bassInit) {
    std::cerr
      << "ERROR: Failed to init BASS library."
      << std::endl;
  }

  _channel = BASS_StreamCreateFile(FALSE, _path.c_str(), 0, 0, BASS_SAMPLE_FLOAT);
  
  if (!_channel) {
    std::cerr << "BASS_StreamCreateFile failed\n";
    BASS_Free();
  }
}

Track::~Track() {
  BASS_StreamFree(_channel);
  BASS_Free();
}

void Track::play() {
  BASS_ChannelPlay(_channel, FALSE);
}

void Track::pause() {
  BASS_ChannelPause(_channel);
}

std::string Track::path() {
  return _path;
}
