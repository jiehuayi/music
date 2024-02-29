#include "CoreCommands.hpp"

void PlayPauseCommand::execute(Parameters args) {
    TRACE_COMMAND();
    _lib.getDisplayPlaylist().trigger();
}

void PlayCommand::execute(Parameters args) {
    TRACE_COMMAND();
    // This should be moved into PlaylistManager instead...
    Playlist& active = _lib.getActivePlaylist();
    Playlist& display = _lib.getDisplayPlaylist();
    if (active != display) {    
        if (active.isPlaying()) {
            active.trigger();
        }
        _lib.setDisplayAsActive();
    }

    _lib.getDisplayPlaylist().play(getListView().getFrom() +
            getListView().getSelectedPosition());
    getVisualView().setRunningMaxFrequency(0.25);
}

void IncVolumeCommand::execute(Parameters args) {
    TRACE_COMMAND();
    _lib.getDisplayPlaylist().incVolume();
}

void DecVolumeCommand::execute(Parameters args) {
    TRACE_COMMAND();
    _lib.getDisplayPlaylist().decVolume();
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
    lv.setSelectedPosition(std::min(
                std::min(lv.getSelectedPosition() + 1,  lv.getHeight() - 3), 
                _lib.getDisplayPlaylist().getSize() - 1));
}

void NavigateUpCommand::execute(Parameters args) {
    TRACE_COMMAND();
    ListComponent& lv = getListView();
    lv.setSelectedPosition(std::max(lv.getSelectedPosition() - 1, 0));
}

void NavigateNextPlaylistCommand::execute(Parameters args) {
    TRACE_COMMAND();
    getListView().clear();
    _lib.nextPlaylist();
}
