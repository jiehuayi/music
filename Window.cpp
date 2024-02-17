#include "Window.hpp"
#include "Log.hpp"

Window::Window(Library& library) : _library(library) {
    setlocale(LC_ALL, "");  
    initscr();
    cbreak();
    noecho();
    raw();
    nodelay(stdscr, true);
    keypad(stdscr, true);
    curs_set(0);
    // start_color();

    init_pair(1, COLOR_BLACK, COLOR_YELLOW);
    init_pair(2, COLOR_WHITE, COLOR_BLACK);
    init_pair(3, COLOR_BLUE, COLOR_BLACK);
    init_pair(4, COLOR_CYAN, COLOR_BLACK);

    getmaxyx(stdscr, _windowY, _windowX);

    _inputMode = MODE_NAVIGATE;
    
    _consoleView = ConsoleComponent(_windowY, _windowX);
    _listView = ListComponent(_windowY, _windowX);
    _visualView = VisualComponent(_windowY, _windowX);
    _popView = PopupComponent(_windowY, _windowX);
}

Window::~Window() {
    endwin();
}

void Window::renderWindow() {
    _consoleView.render(_library);
    _visualView.render(_library);
    _listView.render(_library);
}

int Window::processInput() {
    Playlist& playlist = _library.getActivePlaylist();
    char in = wgetch(stdscr);
    std::string input = "";

    if (_inputMode == MODE_COMMAND) {
        input = _consoleView.getInputBuffer();
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
            _consoleView.setInputBuffer("");
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
            _consoleView.setInputBuffer("");
            break;

        case 0x7F:
        case 0x08:
            _consoleView.setInputBuffer(input.substr(0, input.length() - 1));
            break;

        default:
            _consoleView.appendInputBuffer(in);
            break;
    }

RET:
    return APP_STATE_RUNNING;
}
