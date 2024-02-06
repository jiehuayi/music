#include "Window.hpp"

Window::Window(int playlistSize) {
  setlocale(LC_ALL, "en_US.UTF-8");  
  initscr();
  cbreak();
  noecho();
  nodelay(stdscr, true);
  // halfdelay(true);
  keypad(stdscr, true);
  start_color();

  init_pair(1, COLOR_BLACK, COLOR_YELLOW);
  init_pair(2, COLOR_WHITE, COLOR_BLACK);
  init_pair(3, COLOR_BLUE, COLOR_BLACK);
  init_pair(4, COLOR_CYAN, COLOR_BLACK);

  getmaxyx(stdscr, _windowY, _windowX);
  
  _inputMode = MODE_NAVIGATE;
  _listStartingIndex = 0;
  _cursorPosition = 0;
  _playlistSize = playlistSize;
  _runningScaleSum = 10.0;
  _runningCount = 1.0;
  
  _listFrameX = 0.4 * _windowX;
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
  int renderableSize = _listFrameY - 2; // accounting for borders
  bool overflow = renderableSize < items.size();

  if (_cursorPosition >= renderableSize - 5 &&
      _listStartingIndex + renderableSize < items.size()) {
    _listStartingIndex++;
    _cursorPosition--; // maintian cursor position
  }
  
  auto entryIt = items.begin() + _listStartingIndex;
  int pos = 0;

  for (auto it = entryIt; it != items.end(); ++it) {
    // int pos = entryIt - it;

    // no more space (pos is zero indexed, so stop when ==)
    if (pos >= renderableSize) {
      break;
    }
    
    std::string display = "";
    auto& entry = *it;

    // account for extension
    if (entry.length() < _listFrameX - 2) {
      display = entry + std::string(_listFrameX - 2 - entry.length(), ' ');
    } else {
      // display song file title in short form
      display = entry.substr(0, _listFrameX - 5) += "...";
    }

    if (pos == _cursorPosition) {
      wattron(_listFrame.get(), A_REVERSE);
    }
    
    mvwprintw(_listFrame.get(), pos + 1, 1,
	      FORMAT_PTR(display.c_str()));
    
    wattroff(_listFrame.get(), A_REVERSE);

    pos++;
  }
}

void Window::renderWindowVisual(Playlist& playlist) {
  std::stringstream progressBarBuffer;
  
  // including decorative square brackets
  int progressBarTotalLength = _visualFrameX - 2;
  int progressBarLength = progressBarTotalLength - 2;

  double fillWhole, fillFrac;
  double posNow = playlist.getPosition();
  double posEnd = playlist.getDuration();
  double progress = (posNow / posEnd) * progressBarLength;

  fillFrac = std::modf(progress, &fillWhole);

  // account for two square brackets at the end of the progress bar
  for (int i = 1; i < progressBarLength + 1; i++) {
    if (progress < 0) {
      progressBarBuffer << "-";
      continue;
    }
    
    if (i <= fillWhole) {
      progressBarBuffer << "\u2588";
    } else if (i == static_cast<int>(std::ceil(progress))) {
      if (fillFrac <= 0.125) {
	progressBarBuffer << "\u258F";
      } else if (fillFrac <= 0.250) {
	progressBarBuffer << "\u258E";
      } else if (fillFrac <= 0.375) {
	progressBarBuffer << "\u258D";
      } else if (fillFrac <= 0.500) {
	progressBarBuffer << "\u258C";
      } else if (fillFrac <= 0.625) {
	progressBarBuffer << "\u258B";
      } else if (fillFrac <= 0.750) {
	progressBarBuffer << "\u258A";
      } else if (fillFrac <= 0.875) {
	progressBarBuffer << "\u2589";
      } else {
	progressBarBuffer << "\u2588";
      }
    } else {
      progressBarBuffer << "-";
    }
  }

  mvwprintw(_visualFrame.get(),
	    _visualFrameY - 4, 1, "[%s]", progressBarBuffer.str().c_str());

  mvwprintw(_visualFrame.get(),
	    _visualFrameY - 3, 1, "[ %s / %s ]",
	    getTimeStamp(posNow).c_str(), getTimeStamp(posEnd).c_str());
  
  mvwprintw(_visualFrame.get(),
	    _visualFrameY - 2, 1, "> %s...",
	    playlist.activeSongName()
	    .substr(0, _visualFrameX - 18).c_str());

  // visualizer
  int visualizerY = _visualFrameY - 6;
  int visualizerX = _visualFrameX - 2;
  std::vector<float> dataBuffer = playlist.getFFT();
  std::vector<float> dataBufferCompact;
  
  // downSample
  // int step = BUFF_SZ / visualizerY;
  // for (int i = 0; i < BUFF_SZ; i+=step) {    
  //   float sum = 0.0;
  //   int j = i;
  //   for (; j < std::min(j + step, BUFF_SZ); j++) {
  //     sum += dataBuffer[j];
  //   }
  //   float avg = sum / (j - i + 1);

  //   if (avg > maxMagnitude) {
  //     maxMagnitude = avg;
  //   }
    
  //   dataBufferCompact.push_back(avg);
  // }
  float maxMagnitude = 0.0;
  for (int i = 0; i < visualizerY; i++) {    
    if (dataBuffer[i] > maxMagnitude) {
      maxMagnitude = dataBuffer[i];
    }  
    dataBufferCompact.push_back(dataBuffer[i]);
  }

  _runningScaleSum += maxMagnitude;
  float runningScaleAverage = _runningScaleSum / _runningCount++;

  int printPositionY = 1;
  for (auto& freq : dataBufferCompact) {
    float height = freq * visualizerX / runningScaleAverage;
    float whole, frac;
    frac = std::modf(height, &whole);

    std::stringstream bar;
    for (int i = 0; i < visualizerX; i++) {
      if (i <= whole) {
	bar << "•";
      } else if (i == std::ceil(height)) {
	bar << "⁍";
      } else {
	bar << " ";
      }
    }

    if (printPositionY % 2) {
      wattron(_visualFrame.get(), COLOR_PAIR(4));
      mvwprintw(_visualFrame.get(), printPositionY++, 1, bar.str().c_str());
      wattroff(_visualFrame.get(), COLOR_PAIR(4));
    } else {
      wattron(_visualFrame.get(), COLOR_PAIR(3));
      mvwprintw(_visualFrame.get(), printPositionY++, 1,
		FORMAT_PTR(bar.str().c_str()));
      wattroff(_visualFrame.get(), COLOR_PAIR(3));
    }
  }

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
  char in = wgetch(_listFrame.get());
  std::string inputBufferString = _inputBuffer.str();

  if (_inputMode == MODE_COMMAND) {
    goto CMD;
  }

  switch(in) {
  case 0x10:
    _cursorPosition = std::max(_cursorPosition - 1, 0);
    break;

  case 0x0E:
    _cursorPosition = std::min(_cursorPosition + 1, _playlistSize - 1);
    break;

  case ':':
    _inputMode = MODE_COMMAND;
    _inputBuffer << in;
    wbkgd(_commandFrame.get(), COLOR_PAIR(1)); // Command color    
    mvwprintw(_commandFrame.get(), 0, 0, _inputBuffer.str().c_str());
    break;

  case 0x0D:
  case 'q':
    playlist.play(_listStartingIndex + _cursorPosition);
    _runningScaleSum = 10.0;
    _runningCount = 1.0;
    break;
    
  case ' ':
    playlist.trigger();
    break;
    
  case 0x1B:
    return APP_STATE_TERMINATED;
  }

  goto RET;

 CMD:
  switch(in) {
  case 0x1B:
  case '!':
    _inputMode = MODE_NAVIGATE;
    _inputBuffer.str("");
    wbkgd(_commandFrame.get(), COLOR_PAIR(2)); // reset color
    werase(_commandFrame.get());
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

std::string Window::getTimeStamp(double timeInSeconds) {
  std::stringstream ssFormat;  
  int hours = static_cast<int>(timeInSeconds / 3600);
  int minutes = static_cast<int>((timeInSeconds - hours * 3600) / 60);
  int seconds = static_cast<int>(timeInSeconds - hours * 3600
				 - minutes * 60);

  // Format hours (if greater than 0)
  if (hours > 0) {
    ssFormat << std::setw(2) << std::setfill('0') << hours << ":";
  }

  // Format minutes
  ssFormat << std::setw(2) << std::setfill('0') << minutes << ":";

  // Format seconds
  ssFormat << std::setw(2) << std::setfill('0') << seconds;

  return ssFormat.str();
}
