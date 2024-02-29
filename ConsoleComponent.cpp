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

void ConsoleComponent::render(PlaylistManager& library __attribute__((unused))) {
    std::string consolePrefix = _state == CONSOLE_STATE_OPEN ? "> " : "";
    std::string consolePostfix = _state == CONSOLE_STATE_OPEN ? "█" : "";
    std::string consolePadding = std::string(
            _x - consolePrefix.length() - _inputBuffer.str().length(), ' ');

    if (_state == CONSOLE_STATE_OPEN) {
        WRAP_COLOR(FRAME_PTR, PColor::ColorConsoleText); 
        WRAP_HIGHLIGHT(FRAME_PTR);
    }

    werase(FRAME_PTR);
    mvwprintw(FRAME_PTR, 0, 0, "%s%s", 
            (consolePrefix + _inputBuffer.str() + consolePostfix).c_str(),
            consolePadding.c_str());

    UNWRAP_COLOR(FRAME_PTR, PColor::ColorConsoleText);
    UNWRAP_HIGHLIGHT(FRAME_PTR);
    
    wnoutrefresh(FRAME_PTR);
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
