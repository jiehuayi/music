#include "Window.hpp"
#include "Log.hpp"

Window::Window(Library& library) : _library(library) {
    setlocale(LC_ALL, "en_US.UTF-8");  
    initscr();
    cbreak();
    noecho();
    raw();
    nodelay(stdscr, true);
    // halfdelay(true);
    keypad(stdscr, true);
    // start_color();

    init_pair(1, COLOR_BLACK, COLOR_YELLOW);
    init_pair(2, COLOR_WHITE, COLOR_BLACK);
    init_pair(3, COLOR_BLUE, COLOR_BLACK);
    init_pair(4, COLOR_CYAN, COLOR_BLACK);

    getmaxyx(stdscr, _windowY, _windowX);

    _inputMode = MODE_NAVIGATE;
    
    _listView = ListComponent(_windowY, _windowX);
    _visualView = VisualComponent(_windowY, _windowX);
    _commandFrameX = _windowX;
    _commandFrameY = 1;

    _inputBuffer.str("");

    WINDOW* commandFrame = newwin(_commandFrameY, _commandFrameX, _windowY - 1, 0);
    _commandFrame = std::unique_ptr<WINDOW, FrameDeleter>(commandFrame);

}

Window::~Window() {
    endwin();
}

void Window::renderWindowTemplate() {
}

void Window::renderWindowList() {
    _listView.render(_library);    
}

void Window::renderWindowVisual() {
    _visualView.render(_library);
}

void Window::renderWindowCursor() {
    curs_set(0);
}

void Window::refreshFrames() {
    wrefresh(_commandFrame.get());
}

static bool isValidCommandChar(char input) {
    return (input > 21 && input < 127) ? true : false;
}

int Window::processInput() {
    Playlist& playlist = _library.getActivePlaylist();
    char in = wgetch(stdscr);
    std::string inputBufferString = _inputBuffer.str();

    if (_inputMode == MODE_COMMAND) {
        goto CMD;
    }

    switch(in) {
        case 'k':
        case 0x10:
            _listView.setSelectedPosition(
                    std::max(_listView.getSelectedPosition() - 1, 0));
            break;

        case 'j':
        case 0x0E:
            _listView.setSelectedPosition(
                    std::min(_listView.getSelectedPosition() + 1, 
                        _listView.getHeight() - 3));
            break;

        case 'r':
            _visualView.setOrientation((_visualView.getOrientation() + 1) % 4);
            break;

        case ':':
            _inputMode = MODE_COMMAND;
            _inputBuffer << in;
            wbkgd(_commandFrame.get(), COLOR_PAIR(1)); // Command color    
            mvwprintw(_commandFrame.get(), 0, 0, _inputBuffer.str().c_str());
            break;

        case 0x0D:
        case 'q':
            playlist.play(_listView.getFrom() + _listView.getSelectedPosition());
            _visualView.setRunningMaxFrequency(0.25);
            break;

        case ' ':
            playlist.trigger();
            break;

        case '+':
            playlist.incVolume();
            break;

        case '-':
            playlist.decVolume();
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
