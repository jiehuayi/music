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

#define _XOPEN_SOURCE_EXTENDED 1
#include <curses.h>

#include "Playlist.hpp"

#define MODE_COMMAND 				0
#define MODE_NAVIGATE				1

#define APP_STATE_TERMINATED	    0
#define APP_STATE_RUNNING			1

#define V_TOP    0
#define V_RIGHT  1
#define V_BOTTOM 2
#define V_LEFT   3

#define FORMAT_PTR(char_ptr) ("%s", char_ptr)

struct FrameDeleter {
    void operator()(WINDOW* frame) {
        delwin(frame);
    }
};

class Window {
    public:
        Window(int playlistSize);
        ~Window();
        void renderWindowTemplate();
        void renderWindowList(std::vector<std::string> items);
        void renderWindowVisual(Playlist& playlist);
        void renderWindowCursor();
        void refreshFrames();
        int processInput(Playlist& playlist);

    private:
        std::vector<std::wstring> visualize(int cy, int cx, 
                std::vector<float>& data);
        void processInputNavigateMode();
        void processInputCommandMode();
        std::string getTimeStamp(double timeInSeconds);

    private:
        int _windowX, _windowY;
        int _inputMode;
        int _listStartingIndex;
        int _visualOrientation;
        int _cursorPosition;
        std::stringstream _inputBuffer;
        int _listFrameX, _listFrameY;
        int _visualFrameX, _visualFrameY;
        int _commandFrameX, _commandFrameY;
        std::unique_ptr<WINDOW, FrameDeleter> _listFrame;
        std::unique_ptr<WINDOW, FrameDeleter> _visualFrame;
        std::unique_ptr<WINDOW, FrameDeleter> _commandFrame;
        int _playlistSize;
        float _runningMaxFreq;
};
