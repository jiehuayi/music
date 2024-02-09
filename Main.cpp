#include <iostream>
#include <memory>
#include <dlfcn.h>

#include "Window.hpp"
#include "Library.hpp"
#include "Playlist.hpp"

#define BASS_DLL_PATH "./libraries/bass/bin/libbass.dylib"

int main(int argc, char** argv) {
    std::shared_ptr<void> bass(dlopen(BASS_DLL_PATH,
                RTLD_LAZY), dlclose);
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
