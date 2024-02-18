// --------------- penelope ---------------

#include <iostream>

#include "Window.hpp"
#include "CommandHandler.hpp"
#include "CoreCommands.hpp"
#include "Library.hpp"
#include "Log.hpp"

static int initCommands(CommandHandler& handler, Window& window, Library& library) {
    int regStatus = 0;
    regStatus |= handler.registerCommand("song-toggle", 
            [&window, &library]() -> Command* { return new PlayPauseCommand(window, 
                library); });

    return regStatus;
}

int main(int argc, char** argv) {
    std::cout << "\033[42m\033[30mGoodbye, penelope.\033[0m" << std::endl;
    Log::clear();

    int fps = 40;
    int delayMillis = 1000 / fps;

    Library lib = Library();

    Window wm = Window(lib);
    CommandHandler ch = CommandHandler(wm);

    if (initCommands(ch, wm, lib) > 0) {
        return EXIT_FAILURE;
    }

    for (;;) {
        wm.renderWindow();
        if (wm.processInput(ch) == APP_STATE_TERMINATED) {
            break;
        }

        napms(delayMillis); // from ncurses;
    }

    return EXIT_SUCCESS;
}
