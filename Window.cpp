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
    _listStartingIndex = 0;
    _visualOrientation = V_BOTTOM;
    _cursorPosition = 0;
    _runningMaxFreq = 0.25;
    _numbered = true;
    
    _listView = ListComponent(_windowY, _windowX);
    _listFrameX = 0.4 * _windowX;
    _listFrameY = _windowY - 1;
    _visualFrameX = _windowX - _listFrameX;
    _visualFrameY = _windowY - 1;
    _commandFrameX = _windowX;
    _commandFrameY = 1;

    _inputBuffer.str("");

    WINDOW* visualFrame = newwin(_visualFrameY, _visualFrameX, 0, _listFrameX);
    WINDOW* commandFrame = newwin(_commandFrameY, _commandFrameX, _windowY - 1, 0);

    _visualFrame = std::unique_ptr<WINDOW, FrameDeleter>(visualFrame);
    _commandFrame = std::unique_ptr<WINDOW, FrameDeleter>(commandFrame);

    keypad(visualFrame, true);
    nodelay(visualFrame, true);
}

Window::~Window() {
    endwin();
}

void Window::renderWindowTemplate() {
    box(_visualFrame.get(), 0, 0);
}

void Window::renderWindowList() {
    _listView.render(_library);    
}

void Window::renderWindowVisual() {
    Playlist& playlist = _library.getActivePlaylist();
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

    // Add extra space in the back of formatted string
    // because it will override (visually) the extra % sign
    // if volume goes up to 100%, then going back down
    // (3 digit to 2 digit number without clearing line)
    mvwprintw(_visualFrame.get(),
            _visualFrameY - 3, 1, "[ %s / %s ] VOL: %d%% ",
            getTimeStamp(posNow).c_str(),
            getTimeStamp(posEnd).c_str(),
            static_cast<int>(playlist.getVolume() * 100));

    mvwprintw(_visualFrame.get(),
            _visualFrameY - 2, 1, "> %s...",
            playlist.activeSongName()
            .substr(0, _visualFrameX - 18).c_str());

    // visualizer
    
    if (playlist.isPlaying()) {
        int visualizerY = _visualFrameY- 6;
        int visualizerX = _visualFrameX - 2;
        std::vector<float> databuffer = playlist.getFFT();

        std::vector<std::wstring> visFrame = 
            visualize(visualizerY, visualizerX, databuffer);

        int cur = 1;
        for (auto& line : visFrame) {
            const wchar_t* cstr = line.c_str();
            if (cur % 2) {
                wattron(_visualFrame.get(), COLOR_PAIR(4));
                mvwaddwstr(_visualFrame.get(), cur++, 1, cstr);
                wattroff(_visualFrame.get(), COLOR_PAIR(4));
            } else {
                wattron(_visualFrame.get(), COLOR_PAIR(3));
                mvwaddwstr(_visualFrame.get(), cur++, 1, cstr);
                wattroff(_visualFrame.get(), COLOR_PAIR(3));
            }
        }
    }

    wrefresh(_visualFrame.get());
}

void Window::renderWindowCursor() {
    curs_set(0);
}

void Window::refreshFrames() {
    wrefresh(_visualFrame.get());
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
            _visualOrientation = (_visualOrientation + 1) % 4;
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
            _runningMaxFreq = 0.25;
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

std::vector<std::wstring> Window::visualize(int cy, int cx, std::vector<float>& data) {
    std::vector<std::wstring> canvas(cy, std::wstring(cx, L' '));
    std::vector<float> dataBufferCompact;

    int growMax = (_visualOrientation == V_BOTTOM ||
            _visualOrientation == V_TOP) ? cy : cx;
    int baseMax = growMax == cy ? cx : cy;

    float maxMagnitude = 0.0;
    for (int i = 0; i < baseMax; ++i) {
        maxMagnitude = maxMagnitude < data[i] ? data[i] : maxMagnitude; 
        dataBufferCompact.push_back(data[i]);
    }

    Log::append("\n ------------------------------------------------------- \n");

    _runningMaxFreq = maxMagnitude > _runningMaxFreq ? maxMagnitude :
        _runningMaxFreq;

    int b = 0;
    bool isRB = _visualOrientation == V_BOTTOM ||
        _visualOrientation == V_RIGHT;

    for (auto& freq : dataBufferCompact) {
        float height = freq * (growMax)/ _runningMaxFreq;
        float whole, frac;
        frac = std::modf(height, &whole);

        Log::append(std::to_string(height) + "\n");
        
        // Base logical index
        int bl;
        
        bool eol = false;
        for (int g = 0; g < growMax && !eol; ++g) {
            wchar_t px;
            int gl;

            if (isRB) {
                bl = b;
                gl = growMax - 1 - g;
            } else {
                bl = baseMax - 1 - b;
                gl = g;
            }

            if (g <= whole) {
                px = L'\u2590';
            } else if (g == std::ceil(height)) {
                px = L'\u2597'; // Need change because orientation matters now...
                eol = true;
            } else {
                break;
            }

            if (_visualOrientation == V_LEFT || _visualOrientation == V_RIGHT) {
                canvas[bl][gl] = px;
            } else {
                canvas[gl][bl] = px;
            }
        }
        ++b;
    }

    return canvas; 
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
