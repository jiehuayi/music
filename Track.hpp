 #pragma once

#include <iostream>
#include <string>

#include <bass.h>

class Track {
public:
  Track(std::string path);
  
  ~Track();

  void play();

  void pause();

  std::string path();
  
private:
  std::string _path;

  HSTREAM _channel;
};
 
