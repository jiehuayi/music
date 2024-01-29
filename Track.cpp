#include "Track.hpp"

static
void trackReadCallback(void* track, Uint8* stream, int streamLength) {
  TrackData* audio = (TrackData*)track;
  
  if (audio->length == 0) {
    return;
  }

  Uint32 length = (Uint32)streamLength;
  length = (length > audio->length) ? audio->length : length;

  SDL_memcpy(stream, audio->pos, length);

  audio->pos += length;
  audio->length -= length;
}

Track::Track(std::string path) {
  _path = path;
  
  if (SDL_LoadWAV(_path.c_str(), &_wavSpec, &_wavStart, &_wavLength) == NULL) {
    // TODO: ERROR HANDLING
    std::cerr << "ERROR: " << _path << " could not be loaded" << std::endl;
  }
  
  _data = {.pos = _wavStart, .length = _wavLength};
  _wavSpec.callback = trackReadCallback;
  _wavSpec.userdata = &_data;
  
  _audioDevice = SDL_OpenAudioDevice(NULL, 0, &_wavSpec, NULL,
				     SDL_AUDIO_ALLOW_ANY_CHANGE);
  if (_audioDevice == 0) {
    // TODO: ERROR HANDLING
    std::cerr << "ERROR: " << SDL_GetError() << std::endl;
  }
}

Track::~Track() {
  SDL_FreeWAV(_wavStart);
  SDL_CloseAudioDevice(_audioDevice);
}

void Track::play() {
  SDL_PauseAudioDevice(_audioDevice, 0);
}

void Track::pause() {
  
}

std::string Track::path() {
  return _path;
}
