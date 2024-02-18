#include "CoreCommands.hpp"

PlayPauseCommand::PlayPauseCommand(Window& ctx, Library& lib) : Command(ctx, lib) {}
PlayPauseCommand::~PlayPauseCommand() {}

void PlayPauseCommand::execute(Parameters args) {
    _lib.getActivePlaylist().trigger();
}
