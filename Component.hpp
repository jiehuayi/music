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

        virtual void setFrame(int winy, int winx) = 0;
        virtual void render(Library& library) = 0;
        virtual void clear();

        virtual int getHeight();
        virtual int getWidth();

    protected:
        void makeFrame();

    protected:
        std::shared_ptr<WINDOW> _frame;
        int _y;
        int _x;

        int _oy;
        int _ox;

        int _winy;
        int _winx;
};
