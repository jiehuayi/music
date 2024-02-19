#pragma once

#include <string>
#include <sstream>

#include "Component.hpp"
#include "Color.hpp"

#define CONSOLE_STATE_OPEN  0
#define CONSOLE_STATE_CLOSE 1

class ConsoleComponent : public ComponentBase {
    public:
        ConsoleComponent();
        ConsoleComponent(int winy, int winx);

        void render(Library& library) override;

        std::string getInputBuffer();

        void appendInputBuffer(char inputCharacter);
        void setInputBuffer(std::string input);
        void setConsoleState(int state);

    private:
        bool isValidCommandChar(char inputCharacter);

    private:
        std::stringstream _inputBuffer;
        int _state;
};
