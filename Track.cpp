#include "Track.hpp"

Track::Track(std::filesystem::path path) : _path(path) {
  _trackPath = _path.string();
  _trackName = _path.filename().string();
  _channel = BASS_StreamCreateFile(FALSE,
				   _trackPath.c_str(),
				   0, 0, BASS_SAMPLE_FLOAT);
  
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

std::filesystem::path Track::path() {
  return _path;
}

std::string Track::name() {
  return _trackName;
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

std::vector<float> Track::getChannelFFT() {
  float fftBuffer[BUFF_SZ];
  std::vector<float> fftBufferLite;
  
  bzero(&fftBuffer, BUFF_SZ);

  if (BASS_ChannelGetData(_channel, fftBuffer,
			  BASS_DATA_FFT8192) == -1) {
    fftBufferLite = std::vector<float>(BUFF_SZ, 0.0);
    goto RET;
  }

  fftBufferLite = std::vector<float>(fftBuffer, fftBuffer + BUFF_SZ);

 RET:
  return fftBufferLite;
}
