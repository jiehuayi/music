#include "TitleComponent.hpp"
#include "Color.hpp"

TitleComponent::TitleComponent() : ComponentBase() {
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
    WRAP_COLOR(FRAME_PTR, PColor::ColorTitleText);
    mvwprintw(FRAME_PTR, 0, 0, "%s", std::string(_x, ' ').c_str());
    mvwprintw(FRAME_PTR, 0, 0, " PWD: %s",
            library.getDisplayPlaylist().getPath().c_str());
    UNWRAP_COLOR(FRAME_PTR, PColor::ColorTitleText);

    wnoutrefresh(FRAME_PTR);
}
