#include "CoreCommands.hpp"

void PlayPauseCommand::execute(Parameters args) {
    TRACE_COMMAND();
    _lib.getActivePlaylist().trigger();
}

void PlayCommand::execute(Parameters args) {
    TRACE_COMMAND();
    _lib.getActivePlaylist().play(getListView().getFrom() +
            getListView().getSelectedPosition());
    getVisualView().setRunningMaxFrequency(0.25);
}

void IncVolumeCommand::execute(Parameters args) {
    TRACE_COMMAND();
    _lib.getActivePlaylist().incVolume();
}

void DecVolumeCommand::execute(Parameters args) {
    TRACE_COMMAND();
    _lib.getActivePlaylist().decVolume();
}

void ShuffleCommand::execute(Parameters args) {
    TRACE_COMMAND();
    _lib.shuffleActivePlaylist();
}

void RotVisualClockCommand::execute(Parameters args) {
    TRACE_COMMAND();
    VisualComponent& vv = getVisualView();
    vv.setOrientation((vv.getOrientation() + 1) % 4);
}

void RotVisualAntiClockCommand::execute(Parameters args) {
    TRACE_COMMAND();
}

void NavigateDownCommand::execute(Parameters args) {
    TRACE_COMMAND();
    ListComponent& lv = getListView();
    lv.setSelectedPosition(std::min(lv.getSelectedPosition() + 1, 
                lv.getHeight() - 3));
}

void NavigateUpCommand::execute(Parameters args) {
    TRACE_COMMAND();
    ListComponent& lv = getListView();
    lv.setSelectedPosition(std::max(lv.getSelectedPosition() - 1, 0));
}
