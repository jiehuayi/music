#include "Color.hpp"

void PColor::setDefaultColor() {
    init_pair(ColorMainBackground, COLOR_BLUE, COLOR_NEUT);
    init_pair(ColorListBackground, COLOR_BLUE, COLOR_NEUT);
    init_pair(ColorListBorder, COLOR_BLUE, COLOR_NEUT);
    init_pair(ColorListText, COLOR_BLUE, COLOR_NEUT);
    init_pair(ColorListTextHighlight, COLOR_BLUE, COLOR_NEUT);
    init_pair(ColorVisualBackground, COLOR_BLUE, COLOR_NEUT);
    init_pair(ColorVisualBorder, COLOR_BLUE, COLOR_NEUT);
    init_pair(ColorVisualBar, COLOR_YELLOW, COLOR_NEUT);
    init_pair(ColorVisualText, COLOR_BLUE, COLOR_NEUT);
    init_pair(ColorConsoleBackground, COLOR_BLUE, COLOR_NEUT);
    init_pair(ColorConsoleHighlight, COLOR_BLUE, COLOR_NEUT);
    init_pair(ColorConsoleText, COLOR_BLUE, COLOR_NEUT);
}
