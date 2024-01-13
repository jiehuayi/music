#include <iostream>

#include "Window.hpp"
#include "Playlist.hpp"

int main() {

  std::cout << "Hello World\n";

  Playlist playlist = Playlist();
  Window window = Window(playlist);

  window.renderWindowTemplate();
  window.renderWindowList();
  window.renderWindowCursor();
  window.refreshFrames();
  window.processInput();


  for (;;) {}

  return 0;

}
