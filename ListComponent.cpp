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

    _y = winy - 1;
    _x = 0.4 * winx;

    _frame = std::shared_ptr<WINDOW>(
            newwin(_y, _x, _oy, _ox), windowDeleter);

    _from = 0;
    _selectedPos = 0;
    _numbered = 0;

    keypad(_frame.get(), true);
    nodelay(_frame.get(), true);
}

void ListComponent::render(Library& library) {
    box(_frame.get(), 0, 0); 

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
        if (entry.length() < _x - 2) {
            display = entry;
        } else {
            // display song file title in short form
            display = entry.substr(0, _x - 5 
                    - prefix.size()) += "...";
        }

        std::string clearLine = std::string(_x - 2, ' ');
        mvwprintw(_frame.get(), pos + 1, 1, clearLine.c_str());

        display = prefix + display;
        (pos == _selectedPos) ? wattron(_frame.get(), A_REVERSE) : 0;
        mvwprintw(_frame.get(), pos + 1, 1, display.c_str());
        wattroff(_frame.get(), A_REVERSE);

        pos++;
    }

    wrefresh(_frame.get());
}
