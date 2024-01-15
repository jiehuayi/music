#include "Window.hpp"

Window::Window(Playlist& playlist) : _playlist(playlist) {
  _playlist.readPlaylist();
  _playlistSize = _playlist.getPlaylistSongs().size();
  
  initscr();
  cbreak();
  noecho();
  nodelay(stdscr, true);
  keypad(stdscr, true);

  getmaxyx(stdscr, _windowY, _windowX);
  
  _inputMode = MODE_NAVIGATE;
  _cursorPosition = 0;
  
  _listFrameX = 0.3 * _windowX;
  _listFrameY = _windowY - 1;
  _visualFrameX = _windowX - _listFrameX;
  _visualFrameY = _windowY - 1;

  WINDOW* listFrame = newwin(_listFrameY, _listFrameX, 0, 0);
  WINDOW* visualFrame = newwin(_visualFrameY, _visualFrameX, 0, _listFrameX);

  _listFrame = std::unique_ptr<WINDOW, FrameDeleter>(listFrame);
  _visualFrame = std::unique_ptr<WINDOW, FrameDeleter>(visualFrame);

  keypad(listFrame, true);
  keypad(visualFrame, true);
  nodelay(listFrame, true);
  nodelay(visualFrame, true);
}

Window::~Window() {
  endwin();
}

void Window::renderWindowTemplate() {
  box(_listFrame.get(), 0, 0);
  box(_visualFrame.get(), 0, 0);
}

void Window::renderWindowList() {
  std::vector<std::string> items = _playlist.getPlaylistSongs();
  int entryIndex = 0;

  for (auto &entry: items) {

	if (entryIndex < _listFrameY - 2) {

	  if (entryIndex == _cursorPosition) wattron(_listFrame.get(), A_REVERSE);
	  
	  mvwprintw(_listFrame.get(), entryIndex + 1, 1, FORMAT_PTR(entry.c_str()));

	  wattroff(_listFrame.get(), A_REVERSE);
	}
	
	entryIndex++;
  }
}

void Window::renderWindowCursor() {
  curs_set(0);
  wmove(_listFrame.get(), 0, 0);
}

void Window::refreshFrames() {
  wrefresh(_listFrame.get());
  wrefresh(_visualFrame.get());
}

void Window::processInput() {
  char in = wgetch(_listFrame.get());

  if (in == 'p') {
	_cursorPosition = std::max(_cursorPosition - 1, 0);		
  }

  else if (in == 'n') {
	_cursorPosition = std::min(_cursorPosition + 1, _playlistSize - 1);
  }  
}
