#pragma once

#include <curses.h>

#define WRAP_COLOR(w, c)    wattron(w, COLOR_PAIR(c));
#define UNWRAP_COLOR(w, c)  wattroff(w, COLOR_PAIR(c));

#define WRAP_HIGHLIGHT(w)   wattron(w, A_REVERSE);
#define UNWRAP_HIGHLIGHT(w) wattroff(w, A_REVERSE);

#define COLOR_NEUT -1
    
namespace PColor {
    enum ColorSections {
        ColorMainBackground = 1,
        ColorListBackground,
        ColorListBorder,
        ColorListText,
        ColorListTextHighlight,
        ColorVisualBackground,
        ColorVisualBorder,
        ColorVisualBar,
        ColorVisualText,
        ColorConsoleBackground,
        ColorConsoleHighlight,
        ColorConsoleText,
    };

    void setDefaultColor();
}
