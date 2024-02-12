// --------------- penelope ---------------
// 
// entry point
//

#include <iostream>

#include "Window.hpp"
#include "Library.hpp"
#include "Playlist.hpp"
#include "Log.hpp"

int main(int argc, char** argv) {
    std::cout << "Goodbye, penelope." << std::endl;

    Log::clear();

    int fps = 60;
    int delayMillis = 1000 / fps;

    Library lib = Library();

    Window window = Window();

    for (;;) {
        Playlist& pl = lib.getActivePlaylist();
        window.renderWindowTemplate();
        window.renderWindowCursor(); 
        window.renderWindowList(pl.getPlaylistSongs());
        window.renderWindowVisual(pl);
        window.refreshFrames();

        int status = window.processInput(pl);

        if (status == APP_STATE_TERMINATED) break;

        napms(delayMillis); // from ncurses;
    }

    return EXIT_SUCCESS;
}
