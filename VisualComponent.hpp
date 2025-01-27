#pragma once

#include <string>
#include <sstream>
#include <cmath>

#include "Component.hpp"
#include "Color.hpp"
#include "Log.hpp"

#define V_TOP    0
#define V_RIGHT  1
#define V_BOTTOM 2
#define V_LEFT   3

class VisualComponent : public ComponentBase {
    public:
        VisualComponent();

        void setFrame() override;
        void render(PlaylistManager& library) override;

        int getOrientation();

        void setOrientation(int orientation);
        void setRunningMaxFrequency(float freq);

    private:
        std::vector<std::wstring> visualize(int cy, int cx, std::vector<float>& data);

        void renderVisualizer(Playlist& playlist);
        void renderControls(Playlist& playlist);

        std::string getTimeStamp(double timeInSeconds);
        wchar_t getBar();
        wchar_t getPartialBar();
        wchar_t getBase();

    private:
        int _orientation;
        double _runningMaxFrequency;
};
