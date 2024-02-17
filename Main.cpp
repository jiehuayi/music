// --------------- penelope ---------------
// 
// entry point
//

#include <iostream>

#include "Window.hpp"
#include "CommandHandler.hpp"
#include "CoreCommands.hpp"
#include "Library.hpp"
#include "Log.hpp"

static int initCommands(CommandHandler& handler, Window& window) {
    int regStatus = 0;
    regStatus |= handler.registerCommand("song-toggle", 
            [&window]() -> Command* { return new PlayPauseCommand(window); });

    return regStatus;
}

int main(int argc, char** argv) {
    std::cout << "Goodbye, penelope." << std::endl;

    Log::clear();

    int fps = 40;
    int delayMillis = 1000 / fps;

    Library lib = Library();

    Window wm = Window(lib);
    CommandHandler ch(wm);

    if (initCommands(ch, wm) > 0) {
        return EXIT_FAILURE;
    }

    for (;;) {
        wm.renderWindow();
        if (wm.processInput() == APP_STATE_TERMINATED) {
            break;
        }

        napms(delayMillis); // from ncurses;
    }

    return EXIT_SUCCESS;
}
