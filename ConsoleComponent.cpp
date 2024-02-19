#include "ConsoleComponent.hpp"

ConsoleComponent::ConsoleComponent() : ComponentBase(0, 0) {
    _oy = 0; _ox = 0; _y = 0; _x = 0;
    _frame = nullptr;
}

ConsoleComponent::ConsoleComponent(int winy, int winx) : ComponentBase(winy, winx) {
    _oy = winy - 1;
    _ox = 0;
    _y = 1;
    _x = winx;

    _frame = std::shared_ptr<WINDOW>(
            newwin(_y, _x, _oy, _ox), windowDeleter);

    _inputBuffer = std::stringstream("");
    _state = CONSOLE_STATE_CLOSE;
}

void ConsoleComponent::render(Library& library) {
    std::string consolePrefix = _state == CONSOLE_STATE_OPEN ? "> " : "";

    werase(_frame.get());
    mvwprintw(_frame.get(), 0, 0, "%s", 
            (consolePrefix + _inputBuffer.str()).c_str());

    wrefresh(_frame.get());
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
