#include "ListComponent.hpp"

ListComponent::ListComponent() : ComponentBase(0, 0) {
    _oy = 0; _ox = 0; _y = 0; _x = 0;
    _from = -1;
    _selectedPos = -1;
    _numbered = -1;
    _frame = nullptr;
}

ListComponent::ListComponent(int winy, int winx) : ComponentBase(winy, winx) {
    _oy = 0;
    _ox = 0;

    _y = winy * 0.4;
    _x = winx;

    _frame = std::shared_ptr<WINDOW>(
            newwin(_y, _x, _oy, _ox), windowDeleter);

    _from = 0;
    _selectedPos = 0;
    _numbered = true;

    keypad(_frame.get(), true);
    nodelay(_frame.get(), true);
}

void ListComponent::render(Library& library) {
    cchar_t left_upper_corner, right_upper_corner, left_bottom_corner, right_bottom_corner;
    setcchar(&left_upper_corner, L"\u256d", COLOR_PAIR(0), 0, nullptr);
    setcchar(&right_upper_corner, L"\u256e", COLOR_PAIR(0), 0, nullptr);
    setcchar(&left_bottom_corner, L"\u2570", COLOR_PAIR(0), 0, nullptr);
    setcchar(&right_bottom_corner, L"\u256f", COLOR_PAIR(0), 0, nullptr);

    cchar_t vline, hline;
    setcchar(&vline, L"\u2502", COLOR_PAIR(0), 0, nullptr);  // Vertical line
    setcchar(&hline, L"\u2500", COLOR_PAIR(0), 0, nullptr);  // Horizontal line

    wborder_set(_frame.get(), &vline, &vline, &hline, &hline,
            &left_upper_corner, &right_upper_corner, 
            &left_bottom_corner, &right_bottom_corner);

    std::vector<std::string> listItems = library.getActivePlaylist()
        .getPlaylistSongs();
    
    int renderY = _y - 2;

    if (_selectedPos >= renderY - 5 &&
            _from + renderY < listItems.size()) {
        _from++;
        _selectedPos--;
    } else if (_selectedPos <= 5 && _from > 0) {
        _from--;
        _selectedPos++;
    }

    auto entryIt = listItems.begin() + _from;
    int pos  = 0;

    for (auto it = entryIt; it != listItems.end(); ++it) {
        // no more space (pos is zero indexed, so stop when ==)
        if (pos >= renderY) {
            break;
        }

        auto& entry = *it; 
        std::string display = "";
        std::string prefix = "";

        if (_numbered) {
           prefix = "[" + std::to_string(pos + _from + 1) + "] ";
        }

        // account for extension
        if (entry.length() < _x - 4) {
            display = entry;
        } else {
            // display song file title in short form
            display = entry.substr(0, _x - 6 
                    - prefix.size()) += "...";
        }

        (pos == _selectedPos) ? wattron(_frame.get(), A_REVERSE) : 0;
        std::string clearLine = std::string(_x - 2, ' ');
        mvwprintw(_frame.get(), pos + 1, 1, clearLine.c_str());

        display = prefix + display;
        mvwprintw(_frame.get(), pos + 1, 1, display.c_str());
        wattroff(_frame.get(), A_REVERSE);

        pos++;
    }

    wrefresh(_frame.get());
}

int ListComponent::getSelectedPosition() {
    return _selectedPos;
}

int ListComponent::getFrom() {
    return _from;
}

void ListComponent::setSelectedPosition(int pos) {
    _selectedPos = pos;
}
