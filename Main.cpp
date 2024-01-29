#include <iostream>

#include "Window.hpp"
#include "Playlist.hpp"

int main(int argc, char** argv) {
  SDL_Init(SDL_INIT_AUDIO);

  Playlist playlist = Playlist();
  Window window = Window(playlist);

  window.renderWindowTemplate();
  window.renderWindowCursor();


  for (;;) {
	window.renderWindowList();
	window.refreshFrames();

	int status = window.processInput();

	if (status == APP_STATE_TERMINATED) break;
  }

  SDL_Quit();

  return EXIT_SUCCESS;

}
