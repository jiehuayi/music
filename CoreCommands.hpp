#pragma once

#include "Window.hpp"
#include "Command.hpp"

class PlayPauseCommand : public Command {
    public:
        PlayPauseCommand(Window& ctx, Library& lib);
        ~PlayPauseCommand() override;
        void execute(Parameters args) override;
};

class IncVolumeCommand : public Command {
    void execute(Parameters args);
};

class DecVolumeCommand : public Command {
    void execute(Parameters args);
};
