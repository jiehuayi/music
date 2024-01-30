#pragma once

#include <iostream>
#include <string>

#include <SDL2/SDL.h>
#include <bass.h>

struct TrackData {
  Uint8* pos;
  Uint32 length;
};

class Track {
public:
  Track(std::string path);
  
  ~Track();

  void play();

  void pause();

  std::string path();
  
private:
  std::string _path;
  
  SDL_AudioSpec _wavSpec;
  Uint8* _wavStart;
  Uint32 _wavLength;

  SDL_AudioDeviceID _audioDevice;

  TrackData _data;
};
