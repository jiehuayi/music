#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include <chrono>
#include <memory>
#include <cstdlib>
#include <vector>
#include <string>
#include <cwchar>
#include <cmath>
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
#include "Color.hpp"

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

class CommandHandler;
class Command;

class Window {
    friend class Command;

    public:
        Window(PlaylistManager& library);
        ~Window();

        void processResize();
        void processRender();
        void processUpdate(CommandHandler& handler);
        int processInput(CommandHandler& handler);

    private:
        void processInputNavigateMode();
        void processInputCommandMode();

    public:
        static Window* inst;

    protected:
        PlaylistManager& _library;
        ConsoleComponent _consoleView;
        ListComponent _listView;
        VisualComponent _visualView;
        PopupComponent _popView;
        ColorManager _colorManager;
 
        int _inputMode;
};
