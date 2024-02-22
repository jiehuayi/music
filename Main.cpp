// --------------- penelope ---------------

#include <iostream>

#include "Window.hpp"
#include "CommandHandler.hpp"
#include "CoreCommands.hpp"
#include "Library.hpp"
#include "Log.hpp"

static void initCommands(CommandHandler& handler, Window& window, Library& library) {
    handler.registerCommand(CMD_SONG_TOGGLE, COMMAND_DEFINE(PlayPauseCommand));
    handler.registerCommand(CMD_SONG_PLAY, COMMAND_DEFINE(PlayCommand));
    handler.registerCommand(CMD_VOLUME_INCREMENT, COMMAND_DEFINE(IncVolumeCommand));
    handler.registerCommand(CMD_VOLUME_DECREMENT, COMMAND_DEFINE(DecVolumeCommand));
    handler.registerCommand(CMD_PLAYLIST_SHUFFLE, COMMAND_DEFINE(ShuffleCommand));
    handler.registerCommand(CMD_NAVIGATE_DOWN, COMMAND_DEFINE(NavigateDownCommand));
    handler.registerCommand(CMD_NAVIGATE_UP, COMMAND_DEFINE(NavigateUpCommand));
}

int main(int argc, char** argv) {
    Log::clear();

    int fps = 40;
    int delayMillis = 1000 / fps;

    Library lib = Library();

    Window wm = Window(lib);
    CommandHandler ch = CommandHandler();

    initCommands(ch, wm, lib);

    for (;;) {
        wm.renderWindow();
        if (wm.processInput(ch) == APP_STATE_TERMINATED) {
            break;
        }

        napms(delayMillis); // from ncurses;
    }

    return EXIT_SUCCESS;
}
