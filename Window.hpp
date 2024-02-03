#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <vector>
#include <string>
#include <cstdlib>
#include <cmath>
#include <csignal>
#include <cctype>

#include <unistd.h>

#include <ncurses.h>

#include "Playlist.hpp"

#define MODE_COMMAND 				0
#define MODE_NAVIGATE				1

#define APP_STATE_TERMINATED	   		0
#define APP_STATE_RUNNING			1

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

  int processInput(Playlist& playlist);

  void refreshFrames();

private:

  void processInputNavigateMode();

  void processInputCommandMode();

private:

  int _windowX, _windowY;

  int _inputMode;
  int _cursorPosition;

  std::stringstream _inputBuffer;

  int _listFrameX, _listFrameY;
  int _visualFrameX, _visualFrameY;
  int _commandFrameX, _commandFrameY;

  std::unique_ptr<WINDOW, FrameDeleter> _listFrame;
  std::unique_ptr<WINDOW, FrameDeleter> _visualFrame;
  std::unique_ptr<WINDOW, FrameDeleter> _commandFrame;

  int _playlistSize;
  float _runningScaleSum;
  float _runningCount;

  // WINDOW* _listFrame;
  // WINDOW* _visualFrame;

};
