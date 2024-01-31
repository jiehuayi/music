#include "Track.hpp"

Track::Track(std::string path) : _path(std::move(path)) {
  _channel = BASS_StreamCreateFile(FALSE, _path.c_str(), 0, 0, BASS_SAMPLE_FLOAT);
  
  if (!_channel) {
    std::cerr << "BASS_StreamCreateFile failed\n";
    BASS_Free();
  }
}

Track::~Track() {
  BASS_StreamFree(_channel);
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

double Track::getDuration() {
  QWORD pos = BASS_ChannelGetLength(_channel, BASS_POS_BYTE);
  
  if (pos < 0) {
    return pos;
  }
  
  return BASS_ChannelBytes2Seconds(_channel, pos);
}

double Track::getPosition() {
  QWORD pos = BASS_ChannelGetPosition(_channel, BASS_POS_BYTE);

  if (pos < 0) {
    return pos;
  }
  
  return BASS_ChannelBytes2Seconds(_channel, pos);
}
