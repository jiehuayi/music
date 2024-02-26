#include "ConsoleComponent.hpp"

ConsoleComponent::ConsoleComponent() : ComponentBase() {
    _inputBuffer = std::stringstream("");
    _state = CONSOLE_STATE_CLOSE;

    setFrame();
}

void ConsoleComponent::setFrame() {
    _oy = LINES - 1;
    _ox = 0;
    _y = 1;
    _x = COLS;

    makeFrame();
}

void ConsoleComponent::render(PlaylistManager& library) {
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
