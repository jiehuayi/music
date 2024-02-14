// --------------- penelope ---------------
// 
// entry point
//

#include <iostream>

#include "Window.hpp"
#include "CommandHandler.hpp"
#include "Library.hpp"
#include "Playlist.hpp"
#include "Log.hpp"

int main(int argc, char** argv) {
    std::cout << "Goodbye, penelope." << std::endl;

    Log::clear();

    int fps = 60;
    int delayMillis = 1000 / fps;

    Library lib = Library();

    Window window = Window(lib);
    CommandHandler commandHandler(window);

    for (;;) {
        window.renderWindowTemplate();
        window.renderWindowCursor(); 
        window.renderWindowList();
        window.renderWindowVisual();
        window.refreshFrames();

        int status = window.processInput();

        if (status == APP_STATE_TERMINATED) break;

        napms(delayMillis); // from ncurses;
    }

    return EXIT_SUCCESS;
}
