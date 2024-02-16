#include "Component.hpp"

void windowDeleter(WINDOW* win) {
    delwin(win);
}

ComponentBase::ComponentBase(int winy, int winx) {
    _winy = winy;
    _winx = winx;
}
