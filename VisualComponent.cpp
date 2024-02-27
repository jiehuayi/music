#include "VisualComponent.hpp"

VisualComponent::VisualComponent() : ComponentBase() {
    setFrame();
    keypad(_frame.get(), true);
    nodelay(_frame.get(), true);

    _orientation = V_BOTTOM;
    _runningMaxFrequency = 0.25;
}

void VisualComponent::setFrame() {
    _oy = LINES * 0.4;
    _ox = 0;

    _y = std::ceil((LINES - 1) * 0.6);
    _x = COLS;

    makeFrame();
}

void VisualComponent::render(PlaylistManager& library) { 
    cchar_t left_upper_corner, right_upper_corner, left_bottom_corner, right_bottom_corner;
    setcchar(&left_upper_corner, L"\u256d", COLOR_PAIR(0), 0, nullptr);
    setcchar(&right_upper_corner, L"\u256e", COLOR_PAIR(0), 0, nullptr);
    setcchar(&left_bottom_corner, L"\u2570", COLOR_PAIR(0), 0, nullptr);
    setcchar(&right_bottom_corner, L"\u256f", COLOR_PAIR(0), 0, nullptr);

    cchar_t vline, hline;
    setcchar(&vline, L"\u2502", COLOR_PAIR(0), 0, nullptr);  // Vertical line
    setcchar(&hline, L"\u2500", COLOR_PAIR(0), 0, nullptr);  // Horizontal line
    
    WRAP_COLOR(_frame.get(), PColor::ColorVisualBorder);
    wborder_set(_frame.get(), &vline, &vline, &hline, &hline,
            &left_upper_corner, &right_upper_corner, 
            &left_bottom_corner, &right_bottom_corner);
    UNWRAP_COLOR(_frame.get(), PColor::ColorVisualBorder);

    Playlist& playlist = library.getDisplayPlaylist();
    renderVisualizer(playlist);
    renderControls(playlist);
        
    wnoutrefresh(_frame.get());
}

void VisualComponent::renderVisualizer(Playlist& playlist) {
    WRAP_BOLD(_frame.get());
    WRAP_COLOR(_frame.get(), PColor::ColorVisualBar)
    if (playlist.isPlaying()) {
        int visualizerY = _y - 6;
        int visualizerX = _x - 2;
        std::vector<float> databuffer = playlist.getFFT();

        std::vector<std::wstring> visFrame = 
            visualize(visualizerY, visualizerX, databuffer);

        int cur = 1;
        for (auto& line : visFrame) {
            const wchar_t* cstr = line.c_str();
            if (cur % 2) {
                mvwaddwstr(_frame.get(), cur++, 1, cstr);
            } else {
                mvwaddwstr(_frame.get(), cur++, 1, cstr);
            }
        }
    }
    UNWRAP_COLOR(_frame.get(), PColor::ColorVisualBar);
    UNWRAP_BOLD(_frame.get());
}

void VisualComponent::renderControls(Playlist& playlist) {
    std::stringstream progressBarBuffer;

    // including decorative square brackets
    int progressBarTotalLength = _x - 2;
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
            progressBarBuffer << "\u2501";
        }
    }

    WRAP_COLOR(_frame.get(), PColor::ColorVisualText);
    mvwprintw(_frame.get(),
            _y - 4, 1, "\u258c%s\u2590", progressBarBuffer.str().c_str());

    // Add extra space in the back of formatted string
    // because it will override (visually) the extra % sign
    // if volume goes up to 100%, then going back down
    // (3 digit to 2 digit number without clearing line)
    mvwprintw(_frame.get(),
            _y - 3, 1, "[ %s / %s ] VOL: %d%% ",
            getTimeStamp(posNow).c_str(),
            getTimeStamp(posEnd).c_str(),
            static_cast<int>(std::ceil(playlist.getVolume() * 100)));

    mvwprintw(_frame.get(),
            _y - 2, 1, "> %s...",
            playlist.activeSongName()
            .substr(0, _x - 18).c_str());

    UNWRAP_COLOR(_frame.get(), PColor::ColorVisualText); 
}

int VisualComponent::getOrientation() {
   return _orientation; 
}

void VisualComponent::setOrientation(int orientation) {
    _orientation = orientation;
}

void VisualComponent::setRunningMaxFrequency(float freq) {
    _runningMaxFrequency = freq;
}

std::vector<std::wstring> VisualComponent::visualize(int cy, int cx, 
        std::vector<float>& data) { 
    std::vector<std::wstring> canvas(cy, std::wstring(cx, L' '));
    std::vector<float> dataBufferCompact;

    int growMax = (_orientation == V_BOTTOM ||
            _orientation == V_TOP) ? cy : cx;
    int baseMax = growMax == cy ? cx : cy;

    float maxMagnitude = 0.0;
    for (int i = 0; i < baseMax; ++i) {
        maxMagnitude = maxMagnitude < data[i] ? data[i] : maxMagnitude; 
        dataBufferCompact.push_back(data[i]);
    }

    _runningMaxFrequency = maxMagnitude > _runningMaxFrequency ? maxMagnitude :
        _runningMaxFrequency;

    int b = 0;
    bool isRB = _orientation == V_BOTTOM ||
        _orientation == V_RIGHT;
    
    pthread_t* threads = new pthread_t[baseMax];
    VisualThreadArg* threadArgs = new VisualThreadArg[baseMax];
    for (auto& freq : dataBufferCompact) {
        VisualThreadArg* arg = threadArgs + b;
        arg->freq = freq;
        arg->maxFrequency = _runningMaxFrequency;
        arg->orientation = _orientation;
        arg->growMax = growMax;
        arg->baseMax = baseMax;
        arg->b = b;
        arg->isRB = isRB;
        arg->canvas = &canvas;

        pthread_create(threads + b, NULL, visualizerWorker, arg);
        ++b;
    }

    for (unsigned long i = 0; i < baseMax; ++i) {
        (void) pthread_join(threads + i, NULL);
    }
    
    delete[] threads;
    delete[] threadArgs;
    return canvas; 
}

std::string VisualComponent::getTimeStamp(double timeInSeconds) {
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

void* visualizerWorker(void* args) {
    VisualThreadArg* vta = static_cast<VisualThreadArg*>(args); 
    float freq = vta->freq;
    float maxFrequency = vta->maxFrequency;
    int orientation = vta->orientation;
    int growMax = vta->growMax;
    int baseMax = vta->baseMax;
    int b = vta->b;
    bool isRB = vta->isRB;
    std::vector<std::string>* canvas = vta->canvas;
    
    float height = freq * (growMax) / maxFrequency;
    float whole, frac;
    frac = std::modf(height, &whole); 

    // Base logical index
    int bl;

    bool eol = false;

    // Bar construction
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

        if (g <= whole && whole != 0.0) {
            px = L'\u2590';
        } else if (g == std::ceil(height) || whole == 0.0) {
            if (frac > 0.35) {
                px = L'\u2597'; // Need change because orientation matters now...
            } else {
                px = whole == 0.0 ? L'_' : L' ';
            }
            eol = true;
        } else {
            break;
        }

        if (orientation == V_LEFT || orientation == V_RIGHT) {
            canvas[bl][gl] = px;
        } else {
            canvas[gl][bl] = px;
        }
    }
}

