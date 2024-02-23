#include "Window.hpp"

Window::Window(Library& library) : _library(library) {
    setlocale(LC_ALL, "");  
    initscr();
    cbreak();
    noecho();
    raw();
    nodelay(stdscr, true);
    keypad(stdscr, true);
    curs_set(0);

    if (has_colors()) {
        use_default_colors();
        start_color();
        _colorManager.setDefaultColor();
    }

    getmaxyx(stdscr, _windowY, _windowX);

    _inputMode = MODE_NAVIGATE;
    
    _consoleView = ConsoleComponent(_windowY, _windowX);
    _listView = ListComponent(_windowY, _windowX);
    _visualView = VisualComponent(_windowY, _windowX);
    _popView = PopupComponent(_windowY, _windowX);

}

Window::~Window() {
    endwin();

    std::string goodByeMessage = "Goodbye, penelope.";
    std::cout << "\033[42m\033[30m" 
        << goodByeMessage << std::string( _windowX - goodByeMessage.size(), ' ')
        << "\033[0m" << std::endl;
}

void Window::processRender() {
    _visualView.render(_library);
    _listView.render(_library);
    _consoleView.render(_library);
    
    // Write all data from curses virtual screen to physical screen
    doupdate();
}

void Window::processUpdate(CommandHandler& handler) {
    Playlist& playlist = _library.getActivePlaylist();

    if (playlist.isFinished()) {
        handler.processCommand("move-down");
        handler.processCommand("song-play");
    }
}

int Window::processInput(CommandHandler& handler) {
    Playlist& playlist = _library.getActivePlaylist();
    char in = wgetch(stdscr);
    std::string input = "";

    if (_inputMode == MODE_COMMAND) {
        input = _consoleView.getInputBuffer();
        goto CMD;
    }

    switch(in) {
        case 'k': case 0x10:
            handler.processCommand("move-up");
            break;
        case 'j': case 0x0E:
            handler.processCommand("move-down");
            break;
        case 'r':
            _visualView.setOrientation((_visualView.getOrientation() + 1) % 4);
            break;
        case ':':
            _inputMode = MODE_COMMAND;
            _consoleView.setConsoleState(CONSOLE_STATE_OPEN);
            break;
        case 0x0D: case 'q':
            handler.processCommand("song-play");
            break;
        case ' ':
            handler.processCommand("song-toggle");
            break;
        case '+':
            handler.processCommand("volume-increment");
            break;
        case '-':
            handler.processCommand("volume-decrement");
            break;
        case 0x1B:
            return APP_STATE_TERMINATED;
    }

    goto RET;

CMD:
    if (in == 0x0A && (input == "q" || input == "quit")) {
        return APP_STATE_TERMINATED;
    }

    switch(in) {
        case 0x1B: case '!':
            _consoleView.setConsoleState(CONSOLE_STATE_CLOSE);
            _inputMode = MODE_NAVIGATE;
            break;
        case 0x7F: case 0x08:
            _consoleView.setInputBuffer(input.substr(0, input.length() - 1));
            break;
        case 0x0A:
            handler.processCommand(input);
            _consoleView.setConsoleState(CONSOLE_STATE_CLOSE);
            _inputMode = MODE_NAVIGATE;
            break;
        default:
            _consoleView.appendInputBuffer(in);
            break;
    }

RET:
    return APP_STATE_RUNNING;
}
