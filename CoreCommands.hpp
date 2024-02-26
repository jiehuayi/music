#pragma once

#include "Window.hpp"
#include "Command.hpp"
#include "Log.hpp"

#define CMD_SONG_TOGGLE             "song-toggle"
#define CMD_SONG_PLAY               "song-play"
#define CMD_VOLUME_DECREMENT        "volume-decrement"
#define CMD_VOLUME_INCREMENT        "volume-increment"
#define CMD_PLAYLIST_SHUFFLE        "playlist-shuffle"

#define CMD_ROTATE_CLOCK            "rotate-clock"
#define CMD_ROTATE_ANTICLOCK        "rotate-anticlock"
#define CMD_NAVIGATE_DOWN           "move-down"
#define CMD_NAVIGATE_UP             "move-up"
#define CMD_NAVIGATE_NEXT_PLAYLIST  "next-playlist"

// This macro should ONLY be used within initCommands
//
// Can potentially modify the macro to accomadate a more flexible environment
// where it can be used as long as a reference of Window and Library are accessible
// within the scope
#define COMMAND_DEFINE(c) \
    [&window,&library]()->Command* { \
        return new c(window, library); \
    }

#define COMMAND_DEFINITION_BUILDER(c) \
    class c : public Command { \
        public: \
            using Command::Command; \
            void execute(Parameters args) override; \
    };

#define __COMMAND__ args.name

#define TRACE_COMMAND() \
    TRACE3("Executing command: " + __COMMAND__);

// An extra semicolon is included in macro definition to be compatible with tne
// auto-indentation of my editor
//
// But the class builder  macro should inherently include a semicolon at the
// end of its definition
COMMAND_DEFINITION_BUILDER(PlayPauseCommand);
COMMAND_DEFINITION_BUILDER(PlayCommand);
COMMAND_DEFINITION_BUILDER(IncVolumeCommand);
COMMAND_DEFINITION_BUILDER(DecVolumeCommand);
COMMAND_DEFINITION_BUILDER(ShuffleCommand);
COMMAND_DEFINITION_BUILDER(RotVisualClockCommand);
COMMAND_DEFINITION_BUILDER(RotVisualAntiClockCommand);
COMMAND_DEFINITION_BUILDER(NavigateDownCommand);
COMMAND_DEFINITION_BUILDER(NavigateUpCommand);
COMMAND_DEFINITION_BUILDER(NavigateNextPlaylistCommand);
