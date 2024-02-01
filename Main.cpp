#include <iostream>
// #include <fcntl.h>

#include "Window.hpp"
#include "Playlist.hpp"

int main(int argc, char** argv) {
  // int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
  
  // if (flags == -1) {
  //   perror("fcntl");
  //   return 1;
  // }

  // if (fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK) == -1) {
  //   perror("fcntl");
  //   return 1;
  // }
  
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
