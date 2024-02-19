#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <vector>
#include <string>
#include <cwchar>
#include <cstdlib>
#include <cmath>
#include <csignal>
#include <cctype>
#include <wchar.h>
#include <unistd.h>

#ifdef __APPLE__

#define _XOPEN_SOURCE_EXTENDED 1
#include <curses.h>

#endif

#include "Library.hpp"
#include "Playlist.hpp"
#include "CommandHandler.hpp"
#include "Log.hpp"
#include "Color.hpp":

#include "Component.hpp"
#include "ConsoleComponent.hpp"
#include "ListComponent.hpp"
#include "VisualComponent.hpp"
#include "PopupComponent.hpp"

#define MODE_COMMAND 				0
#define MODE_NAVIGATE				1

#define APP_STATE_TERMINATED	    0
#define APP_STATE_RUNNING			1

#define V_TOP                       0
#define V_RIGHT                     1
#define V_BOTTOM                    2
#define V_LEFT                      3

#define FORMAT_PTR(char_ptr) ("%s", char_ptr)

struct FrameDeleter {
    void operator()(WINDOW* frame) {
        delwin(frame);
    }
};

class CommandHandler;
class Command;

class Window {
    friend class Command;

    public:
        Window(Library& library);
        ~Window();

        void renderWindow();
        int processInput(CommandHandler& handler);

    private:
        void processInputNavigateMode();
        void processInputCommandMode();

    protected:
        Library& _library;
        ConsoleComponent _consoleView;
        ListComponent _listView;
        VisualComponent _visualView;
        PopupComponent _popView;
 
        int _inputMode;

    private:
        int _windowX, _windowY;
};
