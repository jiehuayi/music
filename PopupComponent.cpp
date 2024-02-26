#include "PopupComponent.hpp"

PopupComponent::PopupComponent() : ComponentBase() {
    _oy = 10;
    _ox = 10;

    _y = 5;
    _x = 15;

    _frame = std::shared_ptr<WINDOW>(
            newwin(_y, _x, _oy, _ox), windowDeleter);
}

void PopupComponent::setFrame() { }

void PopupComponent::render(PlaylistManager& library) { 
    cchar_t left_upper_corner, right_upper_corner, left_bottom_corner, right_bottom_corner;
    setcchar(&left_upper_corner, L"\u256d", COLOR_PAIR(0), 0, nullptr);
    setcchar(&right_upper_corner, L"\u256e", COLOR_PAIR(0), 0, nullptr);
    setcchar(&left_bottom_corner, L"\u2570", COLOR_PAIR(0), 0, nullptr);
    setcchar(&right_bottom_corner, L"\u256f", COLOR_PAIR(0), 0, nullptr);

    cchar_t vline, hline;
    setcchar(&vline, L"\u2502", COLOR_PAIR(0), 0, nullptr);  // Vertical line
    setcchar(&hline, L"\u2500", COLOR_PAIR(0), 0, nullptr);  // Horizontal line

    wborder_set(_frame.get(), &vline, &vline, &hline, &hline,
            &left_upper_corner, &right_upper_corner, 
            &left_bottom_corner, &right_bottom_corner);

    mvwprintw(_frame.get(), 1, 1, "Hello World");

    wnoutrefresh(_frame.get());
}
