#include <iostream>

#include "Window.hpp"
#include "Playlist.hpp"

int main() {

  std::cout << "Hello World\n";

  Playlist playlist = Playlist();
  Window window = Window(playlist);

  window.renderWindowTemplate();
  window.renderWindowCursor();


  for (;;) {
	window.renderWindowList();
	window.processInput();

	window.refreshFrames();
  }

  return 0;

}
