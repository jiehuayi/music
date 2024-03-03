#include "Color.hpp"

ColorManager::ColorManager() {}
ColorManager::~ColorManager() {}

void ColorManager::setDefaultColor() {
    using namespace PColor;
    init_pair(ColorMainBackground, COLOR_BLUE, COLOR_NEUT);
    init_pair(ColorListBackground, COLOR_BLUE, COLOR_NEUT);
    init_pair(ColorListBorder, COLOR_MAGENTA, COLOR_NEUT);
    init_pair(ColorListText, COLOR_BLUE, COLOR_NEUT);
    init_pair(ColorListTextHighlight, COLOR_BLUE, COLOR_NEUT);
    init_pair(ColorVisualBackground, COLOR_BLUE, COLOR_NEUT);
    init_pair(ColorVisualBorder, COLOR_MAGENTA, COLOR_NEUT);
    init_pair(ColorVisualBar, COLOR_MAGENTA, COLOR_NEUT);
    init_pair(ColorVisualText, COLOR_BLUE, COLOR_NEUT);
    init_pair(ColorConsoleBackground, COLOR_BLUE, COLOR_NEUT);
    init_pair(ColorConsoleHighlight, COLOR_BLUE, COLOR_NEUT);
    init_pair(ColorConsoleText, COLOR_YELLOW, COLOR_NEUT);
    init_pair(ColorTitleText, COLOR_MAGENTA, COLOR_NEUT);
    init_pair(ColorTitleHighlight, COLOR_RED, COLOR_NEUT);
}

void ColorManager::setColor(PColor::ColorSections section,
        int colorF, int colorB) {
    if (static_cast<int>(section) >= PColor::ColorCount) {
        return;
    }
    init_pair(section, colorF, colorB);
}

