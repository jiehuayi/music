// --------------- penelope ---------------

#include <iostream>

#include "Window.hpp"
#include "CommandHandler.hpp"
#include "CoreCommands.hpp"
#include "Library.hpp"
#include "Log.hpp"

Window* Window::inst = nullptr;

constexpr int TARGET_FPS = 45;
constexpr int SLEEP_DURATION = 1000 / TARGET_FPS; // Milliseconds

static void initCommands(CommandHandler& handler, Window& window, PlaylistManager& library) {
    handler.registerCommand(CMD_SONG_TOGGLE, COMMAND_DEFINE(PlayPauseCommand));
    handler.registerCommand(CMD_SONG_PLAY, COMMAND_DEFINE(PlayCommand));
    handler.registerCommand(CMD_VOLUME_INCREMENT, COMMAND_DEFINE(IncVolumeCommand));
    handler.registerCommand(CMD_VOLUME_DECREMENT, COMMAND_DEFINE(DecVolumeCommand));
    handler.registerCommand(CMD_PLAYLIST_SHUFFLE, COMMAND_DEFINE(ShuffleCommand));
    handler.registerCommand(CMD_NAVIGATE_DOWN, COMMAND_DEFINE(NavigateDownCommand));
    handler.registerCommand(CMD_NAVIGATE_UP, COMMAND_DEFINE(NavigateUpCommand));
    handler.registerCommand(CMD_NAVIGATE_NEXT_PLAYLIST, 
            COMMAND_DEFINE(NavigateNextPlaylistCommand));

    handler.registerAlias(CMD_PLAYLIST_SHUFFLE, "shuffle,sh");
}

int main(int argc, char** argv) {
    TRACE_INIT();
    TRACE1("Executing: " + std::string(*argv));
    TRACE1("Number of arguments given: " + std::to_string(argc));

    PlaylistManager pm = PlaylistManager();
    Window wm = Window(pm);
    Window::inst = &wm;
    CommandHandler ch = CommandHandler();

    initCommands(ch, wm, pm);

    for (;;) {
        wm.processRender();
        wm.processUpdate(ch);

        if (wm.processInput(ch) == APP_STATE_TERMINATED) {
            break;
        }

        napms(SLEEP_DURATION); 
    }

    return EXIT_SUCCESS;
}
