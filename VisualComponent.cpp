#include "VisualComponent.hpp"

VisualComponent::VisualComponent() : ComponentBase() {
    setFrame();
    keypad(FRAME_PTR, true);
    nodelay(FRAME_PTR, true);

    _orientation = V_BOTTOM;
    _runningMaxFrequency = 0.25;
}

void VisualComponent::setFrame() {
    _oy = std::floor((LINES - 2) * 0.4) + 1;
    _ox = 0;

    _y = std::ceil((LINES - 2) * 0.6);
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
    
    WRAP_COLOR(FRAME_PTR, PColor::ColorVisualBorder);
    wborder_set(FRAME_PTR, &vline, &vline, &hline, &hline,
            &left_upper_corner, &right_upper_corner, 
            &left_bottom_corner, &right_bottom_corner);
    UNWRAP_COLOR(FRAME_PTR, PColor::ColorVisualBorder);

    Playlist& playlist = library.getActivePlaylist();
    renderControls(playlist);
    renderVisualizer(playlist);
        
    wnoutrefresh(FRAME_PTR);
}

void VisualComponent::renderVisualizer(Playlist& playlist) {
    if (!playlist.isPlaying()) {
        return;
    }

    WRAP_BOLD(FRAME_PTR);
    WRAP_COLOR(FRAME_PTR, PColor::ColorVisualBar)
        int visualizerY = _y - 6;
    int visualizerX = _x - 2;
    std::vector<float> databuffer = playlist.getFFT();
    std::vector<std::wstring> visFrame = 
        visualize(visualizerY, visualizerX, databuffer);

    int cur = 1;
    for (auto& line : visFrame) {
        const wchar_t* cstr = line.c_str();
        mvwaddwstr(FRAME_PTR, cur++, 1, cstr);
    }

    UNWRAP_COLOR(FRAME_PTR, PColor::ColorVisualBar);
    UNWRAP_BOLD(FRAME_PTR);
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

    WRAP_COLOR(FRAME_PTR, PColor::ColorVisualText);
    mvwprintw(FRAME_PTR,
            _y - 4, 1, "\u258c%s\u2590", progressBarBuffer.str().c_str());

    // Add extra space in the back of formatted string
    // because it will override (visually) the extra % sign
    // if volume goes up to 100%, then going back down
    // (3 digit to 2 digit number without clearing line)
    mvwprintw(FRAME_PTR,
            _y - 3, 1, "[ %s / %s ] VOL: %d%% ",
            getTimeStamp(posNow).c_str(),
            getTimeStamp(posEnd).c_str(),
            static_cast<int>(std::ceil(playlist.getVolume() * 100)));

    mvwprintw(FRAME_PTR, _y - 2, 1, "%s", std::string(_x - 2, ' ').c_str());
    mvwprintw(FRAME_PTR, _y - 2, 1, "> %s...",
            playlist.activeSongName().substr(0, _x - 18).c_str());

    UNWRAP_COLOR(FRAME_PTR, PColor::ColorVisualText); 
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

    int growMax = (_orientation == V_BOTTOM || _orientation == V_TOP) ? cy : cx;
    int baseMax = growMax == cy ? cx : cy;
    float maxMagnitude = *std::max_element(data.begin(), data.begin() + baseMax);

    _runningMaxFrequency = maxMagnitude > _runningMaxFrequency ? maxMagnitude :
        _runningMaxFrequency;

    int b = 0;
    bool isRB = _orientation == V_BOTTOM ||
        _orientation == V_RIGHT;

    for (auto& freq : data) {
        float height = freq * (growMax)/ _runningMaxFrequency;
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
                px = getBar();
            } else if (g == std::ceil(height) || whole == 0.0) {
                if (frac > 0.35) {
                    px = getPartialBar(); 
                } else {
                    px = whole == 0.0 ? getBase() : L' ';
                }
                eol = true;
            } else {
                break;
            }

            if (_orientation == V_LEFT || _orientation == V_RIGHT) {
                canvas[bl][gl] = px;
            } else {
                canvas[gl][bl] = px;
            }
        }
        ++b;

        if (b == baseMax) break;
    }

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

wchar_t VisualComponent::getBar() {
    switch(_orientation) {
        case V_BOTTOM:
            return L'\u2590';
        case V_TOP:
            return L'\u2590';
        case V_LEFT:
            return L'\u2580';
        case V_RIGHT:
            return L'\u2580';
    }
    return ' ';
}


wchar_t VisualComponent::getPartialBar() {
    switch(_orientation) {
        case V_BOTTOM:
            return L'\u2597';
        case V_TOP:
            return L'\u259d';
        case V_LEFT:
            return L'\u2598';
        case V_RIGHT:
            return L'\u259d';
    }
    return ' ';
}

wchar_t VisualComponent::getBase() {
    switch(_orientation) {
        case V_BOTTOM:
            return L'_';
        case V_TOP:
            return L' ';
        case V_LEFT:
            return L' ';
        case V_RIGHT:
            return L' ';
    }
    return L' ';
}
