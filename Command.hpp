#pragma once

#include <iostream>

#include "Window.hpp"

class Command {
    public:
        Command(Window& ctx);
        ~Command();

        virtual void execute() = 0;

    private:
        Window& _ctx;
};
