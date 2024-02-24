#include "ConsoleComponent.hpp"

ConsoleComponent::ConsoleComponent() : ComponentBase(0, 0) {
    _oy = 0; _ox = 0; _y = 0; _x = 0;
    _frame = nullptr;
}

ConsoleComponent::ConsoleComponent(int winy, int winx) : ComponentBase(winy, winx) {
    _inputBuffer = std::stringstream("");
    _state = CONSOLE_STATE_CLOSE;

    setFrame(winy, winx);
}

void ConsoleComponent::setFrame(int winy, int winx) {
    if (winy != _winy || winx != _winx) {
        _winy = winy;
        _winx = winx;
    }

    wclear(_frame.get());

    _oy = winy - 1;
    _ox = 0;
    _y = 1;
    _x = winx;

    _frame = std::shared_ptr<WINDOW>(
            newwin(_y, _x, _oy, _ox), windowDeleter);
}

void ConsoleComponent::render(Library& library) {
    std::string consolePrefix = _state == CONSOLE_STATE_OPEN ? "> " : "";
    std::string consolePostfix = _state == CONSOLE_STATE_OPEN ? "█" : "";
    std::string consolePadding = std::string(
            _x - consolePrefix.length() - _inputBuffer.str().length(), ' ');

    if (_state == CONSOLE_STATE_OPEN) {
        WRAP_COLOR(_frame.get(), PColor::ColorConsoleText); 
        WRAP_HIGHLIGHT(_frame.get());
    }

    werase(_frame.get());
    mvwprintw(_frame.get(), 0, 0, "%s%s", 
            (consolePrefix + _inputBuffer.str() + consolePostfix).c_str(),
            consolePadding.c_str());

    UNWRAP_COLOR(_frame.get(), PColor::ColorConsoleText);
    UNWRAP_HIGHLIGHT(_frame.get());
    
    wnoutrefresh(_frame.get());
}

bool ConsoleComponent::isValidCommandChar(char inputCharacter) {
    return (inputCharacter > 21 && inputCharacter < 127) ? true : false;
}

std::string ConsoleComponent::getInputBuffer() {
    return _inputBuffer.str();
}

void ConsoleComponent::appendInputBuffer(char inputCharacter) {
    if (!isValidCommandChar(inputCharacter)) {
        return;
    }

    _inputBuffer << inputCharacter;
}

void ConsoleComponent::setInputBuffer(std::string input) {
    _inputBuffer.str("");
    _inputBuffer << input;
}

void ConsoleComponent::setConsoleState(int state) {
    if (state == CONSOLE_STATE_CLOSE) {
        setInputBuffer("");
    }

    _state = state;
}
