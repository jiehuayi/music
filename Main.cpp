#include <iostream>
#include <memory>
#include <dlfcn.h> // dlopen(), dlsym, dlclose()

#include "Window.hpp"
#include "Library.hpp"
#include "Playlist.hpp"

int main(int argc, char** argv) {
    int fps = 60;
    int delayMillis = 1000 / fps;

    Library lib = Library();

    Window window = Window(lib.getActivePlaylist().size());

    for (;;) {
        Playlist& pl = lib.getActivePlaylist();
        window.renderWindowTemplate();
        window.renderWindowCursor(); 
        window.renderWindowList(pl.getPlaylistSongs());
        window.renderWindowVisual(pl);
        window.refreshFrames();

        int status = window.processInput(pl);

        if (status == APP_STATE_TERMINATED) break;
        napms(delayMillis);
    }

    return EXIT_SUCCESS;

}
