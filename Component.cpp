#include "Component.hpp"

void windowDeleter(WINDOW* win) {
    if (win == nullptr) {
        return;
    }

    delwin(win);
}

ComponentBase::ComponentBase() {
    _frame = nullptr;
}

void ComponentBase::clear() {
    if (_frame) {
        wclear(_frame.get());
        wrefresh(_frame.get());
    }
}

int ComponentBase::getHeight() { return _y; }
int ComponentBase::getWidth() { return _x; }

void ComponentBase::makeFrame() {
    if (_frame.get()) {
        wclear(_frame.get());
        wrefresh(_frame.get());
        _frame.reset();
    }
    
    _frame = std::shared_ptr<WINDOW>(
            newwin(_y, _x, _oy, _ox), windowDeleter);
}
