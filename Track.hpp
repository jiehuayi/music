#pragma once

#include <iostream>
#include <filesystem>
#include <string>
#include <cstring>

#include <bass.h>

#define BUFF_SZ 512

class Track {
  
public:
  Track(std::filesystem::path path);
  
  ~Track();

  void play();

  void pause();

  std::filesystem::path path();

  std::string name();

  double getDuration();

  double getPosition();

  std::vector<float> getChannelFFT();
  
private:
  std::filesystem::path _path;
  std::string _trackPath;
  std::string _trackName;

  HSTREAM _channel;
};
 
