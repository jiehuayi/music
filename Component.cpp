#include "Component.hpp"

void windowDeleter(WINDOW* win) {
    if (win == nullptr) {
        return;
    }

    delwin(win);
}

ComponentBase::ComponentBase(int winy, int winx) {
    _winy = winy;
    _winx = winx;
}

int ComponentBase::getHeight() { return _y; }
int ComponentBase::getWidth() { return _x; }