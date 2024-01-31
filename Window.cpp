#include "Window.hpp"

Window::Window(int playlistSize) {
  initscr();
  setlocale(LC_ALL, "");  
  cbreak();
  noecho();
  nodelay(stdscr, true);
  keypad(stdscr, true);
  start_color();

  init_pair(2, COLOR_WHITE, COLOR_BLACK);
  init_pair(1, COLOR_BLACK, COLOR_YELLOW);

  getmaxyx(stdscr, _windowY, _windowX);
  
  _inputMode = MODE_NAVIGATE;
  _cursorPosition = 0;
  _playlistSize = playlistSize;
  
  _listFrameX = 0.3 * _windowX;
  _listFrameY = _windowY - 1;
  _visualFrameX = _windowX - _listFrameX;
  _visualFrameY = _windowY - 1;
  _commandFrameX = _windowX;
  _commandFrameY = 1;

  _inputBuffer.str("");

  WINDOW* listFrame = newwin(_listFrameY, _listFrameX, 0, 0);
  WINDOW* visualFrame = newwin(_visualFrameY, _visualFrameX, 0, _listFrameX);
  WINDOW* commandFrame = newwin(_commandFrameY, _commandFrameX, _windowY - 1, 0);

  _listFrame = std::unique_ptr<WINDOW, FrameDeleter>(listFrame);
  _visualFrame = std::unique_ptr<WINDOW, FrameDeleter>(visualFrame);
  _commandFrame = std::unique_ptr<WINDOW, FrameDeleter>(commandFrame);

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

void Window::renderWindowList(std::vector<std::string> items) {
  int entryIndex = 0;

  for (auto &entry: items) {
    if (entryIndex < _listFrameY - 2) {
      if (entryIndex == _cursorPosition) wattron(_listFrame.get(), A_REVERSE);

      std::string display = "";

      if (entry.length() <= _listFrameX - 2) {
	display = entry;
      } else {
	display = entry.substr(0, _listFrameX - 5) += "...";
      }
	  
      mvwprintw(_listFrame.get(), entryIndex + 1, 1, FORMAT_PTR(display.c_str()));
      
      wattroff(_listFrame.get(), A_REVERSE);
    }	
    entryIndex++;
  }
}

void Window::renderWindowVisual(Playlist& playlist) {
  std::stringstream progressBarBuffer;
  
  // including decorative square brackets
  int progressBarTotalLength = _visualFrameX - 2;
  int progressBarLength = progressBarTotalLength - 2;
  int fillAmount = std::floor(playlist.progress() * progressBarLength);

  std::ofstream outFile("log.log", std::ios::app);
  outFile << playlist.progress() << "\n";
  outFile.close();

  // account for two square brackets at the end of the progress bar
  for (int i = 0; i < progressBarLength; i++) {
    if (i <= fillAmount)
      progressBarBuffer << "0";
    else
      progressBarBuffer << "-";
  }

  mvwprintw(_visualFrame.get(),
	    _visualFrameY - 2, 1, "[%s]", progressBarBuffer.str().c_str());

  wrefresh(_visualFrame.get());
}

void Window::renderWindowCursor() {
  curs_set(0);
  wmove(_listFrame.get(), 0, 0);
}

void Window::refreshFrames() {
  wrefresh(_listFrame.get());
  wrefresh(_visualFrame.get());
  wrefresh(_commandFrame.get());
  // mvwprintw(_commandFrame.get(), 0, 0, "%d", rand());
}

static bool isValidCommandChar(char input) {
  return (input > 21 && input < 127) ? true : false;
}

int Window::processInput(Playlist& playlist) {
  // char buffer[1024];
  // ssize_t size = read(STDIN_FILENO, buffer, sizeof(buffer) - 1); // read stdin

  // if (size == -1 || size == 0) {
  //   return APP_STATE_RUNNING;
  // }

  // char in = buffer[0];
  char in = wgetch(_listFrame.get());
  std::string inputBufferString = _inputBuffer.str();

  // mvwprintw(_commandFrame.get(), 0, 0, "%x", in); // debugging only

  if (_inputMode == MODE_COMMAND) goto CMD;

  switch(in) {
  case 0x10:
    _cursorPosition = std::max(_cursorPosition - 1, 0);
    break;

  case 0x0E:
    _cursorPosition = std::min(_cursorPosition + 1, _playlistSize - 1);
    break;

  case ':':
    _inputMode = MODE_COMMAND;
    wbkgd(_commandFrame.get(), COLOR_PAIR(1)); // command color
    
    _inputBuffer << in;
    mvwprintw(_commandFrame.get(), 0, 0, "%s", _inputBuffer.str().c_str());
    break;

  case 0x0D:
  case 'q':
    playlist.play(_cursorPosition);
    break;
    
  case ' ':
    playlist.trigger();
    break;
    
  case 0x1B:
    return APP_STATE_TERMINATED;

  default:
    goto RET;
  }

 CMD:
  switch(in) {
  case 0x1B:
    _inputMode = MODE_NAVIGATE;
    wbkgd(_commandFrame.get(), COLOR_PAIR(2)); // reset color
    break;

  case 0x7F:
  case 0x08:
    if (inputBufferString.length() > 1) {
      _inputBuffer.str(std::string());
      _inputBuffer << inputBufferString
		       .substr(0, inputBufferString.length() - 1);
    }

    werase(_commandFrame.get());
    mvwprintw(_commandFrame.get(), 0, 0, "%s", _inputBuffer.str().c_str());
    break;

  default:
    if (isValidCommandChar(in)) {
      _inputBuffer << in;
      mvwprintw(_commandFrame.get(), 0, 0, "%s", _inputBuffer.str().c_str());
    }  
    break;
  }
  
 RET:
  return APP_STATE_RUNNING;
}
