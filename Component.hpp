#pragma once

#include <memory>

#ifdef __APPLE__

#define _XOPEN_SOURCE_EXTENDED 1
#include <curses.h>

#endif

#include "Library.hpp"

void windowDeleter(WINDOW* win);

class ComponentBase {
    public:
        ComponentBase(int winy, int winx);

        virtual void render(Library& library) = 0;

        int getHeight();
        int getWidth();

    protected:
        std::shared_ptr<WINDOW> _frame;
        int _y;
        int _x;

        int _oy;
        int _ox;

        int _winy;
        int _winx;
};
