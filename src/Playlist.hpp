#include <iostream>
#include <filesystem>
#include <string>
#include <vector>

using std::string

class Playlist {

public:
  
  Playlist();

  Playlist(string root);

  ~Playlist();

  void readPlaylist();

  std::vector<string> getPlaylistNames();

private:

  static const string _DEFAULT_ROOT = "~/Music/";

  string _root;
  
  std::vector<string> _songs;

};
