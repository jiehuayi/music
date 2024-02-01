#pragma once

#include <iostream>
#include <filesystem>
#include <string>

#include <bass.h>

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
  
private:
  std::filesystem::path _path;
  std::string _trackPath;
  std::string _trackName;

  HSTREAM _channel;
};
 
