// --------------- penelope ---------------
// 
// entry point
//

#include <iostream>

#include "Window.hpp"
#include "CommandHandler.hpp"
#include "Library.hpp"
#include "Log.hpp"

int main(int argc, char** argv) {
    std::cout << "Goodbye, penelope." << std::endl;

    Log::clear();

    int fps = 40;
    int delayMillis = 1000 / fps;

    Library lib = Library();

    Window wm = Window(lib);
    CommandHandler commandHandler(wm);

    for (;;) {
        wm.renderWindow();
        if (wm.processInput() == APP_STATE_TERMINATED) {
            break;
        }

        napms(delayMillis); // from ncurses;
    }

    return EXIT_SUCCESS;
}
