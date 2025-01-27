#include "ListComponent.hpp"

ListComponent::ListComponent() : ComponentBase() {
    _from = 0;
    _selectedPos = 0;
    _numbered = true;

    setFrame();
}

void ListComponent::setFrame() {
    _oy = 1;
    _ox = 0;
    _y = std::floor((LINES - 2) * 0.4);
    _x = COLS;

    makeFrame();
}

void ListComponent::render(PlaylistManager& library) {
    cchar_t left_upper_corner, right_upper_corner, left_bottom_corner, right_bottom_corner;
    setcchar(&left_upper_corner, L"\u256d", COLOR_PAIR(0), 0, nullptr);
    setcchar(&right_upper_corner, L"\u256e", COLOR_PAIR(0), 0, nullptr);
    setcchar(&left_bottom_corner, L"\u2570", COLOR_PAIR(0), 0, nullptr);
    setcchar(&right_bottom_corner, L"\u256f", COLOR_PAIR(0), 0, nullptr);

    cchar_t vline, hline;
    setcchar(&vline, L"\u2502", COLOR_PAIR(0), 0, nullptr);  // Vertical line
    setcchar(&hline, L"\u2500", COLOR_PAIR(0), 0, nullptr);  // Horizontal line
    
    WRAP_COLOR(FRAME_PTR, PColor::ColorListBorder);

    wborder_set(FRAME_PTR, &vline, &vline, &hline, &hline,
            &left_upper_corner, &right_upper_corner, 
            &left_bottom_corner, &right_bottom_corner);

    UNWRAP_COLOR(FRAME_PTR, PColor::ColorListBorder);

    std::vector<std::string> listItems = library.getDisplayPlaylist()
        .getPlaylistSongs();
    
    int renderY = _y - 2;
    int scrollPadding = renderY < 6 ? 1 : 5;

    if (_selectedPos >= renderY - scrollPadding &&
            _from + renderY < static_cast<int>(listItems.size())) {
        _from++;
        _selectedPos--;
    } else if (_selectedPos <= scrollPadding && _from > 0) {
        _from--;
        _selectedPos++;
    }

    auto entryIt = listItems.begin() + _from;
    int pos  = 0;
    
    WRAP_COLOR(FRAME_PTR, PColor::ColorListText);
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
        if (static_cast<int>(entry.length()) < _x - 4) {
            display = entry;
        } else {
            // display song file title in short form
            display = entry.substr(0, _x - 6 
                    - prefix.size()) += "...";
        }
        
        if (pos == _selectedPos) {
            WRAP_HIGHLIGHT(FRAME_PTR);
        }

        std::string clearLine = std::string(_x - 2, ' ');
        mvwprintw(FRAME_PTR, pos + 1, 1, FORMAT_CSTR(clearLine.c_str()));

        display = prefix + display;
        mvwprintw(FRAME_PTR, pos + 1, 1, FORMAT_CSTR(display.c_str()));
        pos++;

        UNWRAP_HIGHLIGHT(FRAME_PTR);

    }
    UNWRAP_COLOR(FRAME_PTR, PColor::ColorListText);

    wnoutrefresh(FRAME_PTR);
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
