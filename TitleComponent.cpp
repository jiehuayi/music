#include "TitleComponent.hpp"
#include "Color.hpp"

TitleComponent::TitleComponent() : ComponentBase() {
    _alerting = false;
    _alertDuration = ALERT_DEFAULT_DURATION;
    setFrame();
}

void TitleComponent::setFrame() {
    _oy = 0;
    _ox = 0;
    _y = 1;
    _x = COLS;

    makeFrame();
}

void TitleComponent::render(PlaylistManager& library) {
    updateInternalState();

    if (_alerting) {
        WRAP_COLOR(FRAME_PTR, PColor::ColorTitleHighlight);
        WRAP_HIGHLIGHT(FRAME_PTR);
        mvwprintw(FRAME_PTR, 0, 0, "%s", std::string(_x, ' ').c_str());
        mvwprintw(FRAME_PTR, 0, 0, " %s", _alertMessage.c_str());
        UNWRAP_HIGHLIGHT(FRAME_PTR);
        UNWRAP_COLOR(FRAME_PTR, PColor::ColorTitleHighlight);
    } else {
        WRAP_COLOR(FRAME_PTR, PColor::ColorTitleText);
        mvwprintw(FRAME_PTR, 0, 0, "%s", std::string(_x, ' ').c_str());
        mvwprintw(FRAME_PTR, 0, 0, " PWD: %s",
                library.getDisplayPlaylist().getPath().c_str());
        UNWRAP_COLOR(FRAME_PTR, PColor::ColorTitleText);
    }

    wnoutrefresh(FRAME_PTR);
}

void TitleComponent::updateInternalState() {
    if (_alerting && _clock.hasElapsed(_alertDuration)) {
        _alerting = false; 
    }
}

void TitleComponent::setAlert(std::string message) {
    _clock.start();
    _alertMessage = message;
    _alerting = true;
}
