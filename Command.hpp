#pragma once

#include "Window.hpp"

struct Parameters {
    std::string name;
    std::vector<std::string> values; 
};

class Command {
    public:
        Command(Window& ctx);
        ~Command();

        virtual void execute(Parameters args) = 0;

    private:
        Window& _ctx;
};
