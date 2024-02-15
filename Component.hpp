#pragma once

#include <memory>

#include <ncurses.h>

#include "Library.hpp"

void windowDeleter(WINDOW* win);

class ComponentBase {
    public:
        ComponentBase(int winy, int winx);

        virtual void render(Library& library) = 0;

    protected:
        std::shared_ptr<WINDOW> _frame;
        int _y;
        int _x;

        int _oy;
        int _ox;

        int _winy;
        int _winx;
};
