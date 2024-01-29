#include <iostream>

#include "Window.hpp"
#include "Playlist.hpp"

int main(int argc, char** argv) {
  SDL_Init(SDL_INIT_AUDIO);

  Playlist playlist = Playlist();
  playlist.readPlaylist();

  Window window = Window(playlist.size());
  window.renderWindowTemplate();
  window.renderWindowCursor();
  
  for (;;) {
    window.renderWindowList(playlist.getPlaylistSongs());
    window.refreshFrames();

    int status = window.processInput(playlist);

    if (status == APP_STATE_TERMINATED) break;
  }

  SDL_Quit();

  return EXIT_SUCCESS;

}
