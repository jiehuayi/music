#include "VisualComponent.hpp"

VisualComponent::VisualComponent() : ComponentBase(-1, -1) {
    _oy = 0; _ox = 0; _y = 0; _x = 0;
}

VisualComponent::VisualComponent(int winy, int winx) : ComponentBase(winy, winx) {
    _oy = 0;
    _ox = winx * 0.4 + 1;

    _y = winy - 1;
    _x = 0.6 * winx;

    _frame = std::shared_ptr<WINDOW>(
            newwin(_y, _x, _oy, _ox), windowDeleter);

    keypad(_frame.get(), true);
    nodelay(_frame.get(), true);

    _orientation = V_BOTTOM;
    _runningMaxFrequency = 0.25;
}

void VisualComponent::render(Library& library) { 
    box(_frame.get(), 0, 0);

    Playlist& playlist = library.getActivePlaylist();
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
            progressBarBuffer << "-";
        }
    }

    mvwprintw(_frame.get(),
            _y - 4, 1, "[%s]", progressBarBuffer.str().c_str());

    // Add extra space in the back of formatted string
    // because it will override (visually) the extra % sign
    // if volume goes up to 100%, then going back down
    // (3 digit to 2 digit number without clearing line)
    mvwprintw(_frame.get(),
            _y - 3, 1, "[ %s / %s ] VOL: %d%% ",
            getTimeStamp(posNow).c_str(),
            getTimeStamp(posEnd).c_str(),
            static_cast<int>(playlist.getVolume() * 100));

    mvwprintw(_frame.get(),
            _y - 2, 1, "> %s...",
            playlist.activeSongName()
            .substr(0, _x - 18).c_str());

    // visualizer
    
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
                wattron(_frame.get(), COLOR_PAIR(4));
                mvwaddwstr(_frame.get(), cur++, 1, cstr);
                wattroff(_frame.get(), COLOR_PAIR(4));
            } else {
                wattron(_frame.get(), COLOR_PAIR(3));
                mvwaddwstr(_frame.get(), cur++, 1, cstr);
                wattroff(_frame.get(), COLOR_PAIR(3));
            }
        }
    }

    wrefresh(_frame.get());
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

    Log::append("\n ------------------------------------------------------- \n");

    _runningMaxFrequency = maxMagnitude > _runningMaxFrequency ? maxMagnitude :
        _runningMaxFrequency;

    int b = 0;
    bool isRB = _orientation == V_BOTTOM ||
        _orientation == V_RIGHT;

    for (auto& freq : dataBufferCompact) {
        float height = freq * (growMax)/ _runningMaxFrequency;
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

            if (_orientation == V_LEFT || _orientation == V_RIGHT) {
                canvas[bl][gl] = px;
            } else {
                canvas[gl][bl] = px;
            }
        }
        ++b;
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
