#pragma once

#include "Parameters.hpp"
#include "Window.hpp"
#include "Library.hpp"

class Window;

class Command {
    public:
        Command(Window& ctx, Library& lib);
        virtual ~Command() = 0;

        virtual void execute(Parameters args) = 0;

    protected:
        Window& _ctx;
        Library& _lib;
};
