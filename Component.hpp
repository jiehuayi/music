#pragma once

#include <memory>

#ifdef __APPLE__

#define _XOPEN_SOURCE_EXTENDED 1
#include <curses.h>

#endif

#include "Library.hpp"

#define FORMAT_CSTR(cp) "%s", cp
#define FRAME_PTR _frame.get()

void windowDeleter(WINDOW* win);

class ComponentBase {
    public:
        ComponentBase();

        virtual void setFrame() = 0;
        virtual void render(PlaylistManager& library) = 0;
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
};
