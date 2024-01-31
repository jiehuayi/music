#include <iostream>

#include "Window.hpp"
#include "Playlist.hpp"

int main(int argc, char** argv) {
  Playlist pl = Playlist();
  pl.readPlaylist();

  Window window = Window(pl.size());
  window.renderWindowTemplate();
  window.renderWindowCursor();
  
  for (;;) {
    window.renderWindowList(pl.getPlaylistSongs());
    window.renderWindowVisual(pl);
    window.refreshFrames();

    int status = window.processInput(pl);

    if (status == APP_STATE_TERMINATED) break;
  }

  return EXIT_SUCCESS;

}
