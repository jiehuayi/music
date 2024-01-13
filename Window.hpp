#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <string>

#include <ncurses.h>

#include "Playlist.hpp"

#define MODE_NAVIGATE				1
#define MODE_COMMAND 				0

#define FORMAT_PTR(char_ptr) ("%s", char_ptr)

struct FrameDeleter {
  void operator()(WINDOW* frame) {
	delwin(frame);
  }
};

class Window {

public:

  Window(Playlist& playlist);

  ~Window();

  void renderWindowTemplate();

  void renderWindowList();

  void renderWindowCursor();

  void processInput();

  void refreshFrames();

private:

  void processInputNavigateMode();

  void processInputCommandMode();

private:

  int _windowX, _windowY;

  int _inputMode;
  int _cursorPosition;

  int _listFrameX, _listFrameY;
  int _visualFrameX, _visualFrameY;

  std::unique_ptr<WINDOW, FrameDeleter> _listFrame;
  std::unique_ptr<WINDOW, FrameDeleter> _visualFrame;

  Playlist _playlist;

  // WINDOW* _listFrame;
  // WINDOW* _visualFrame;

};
