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
        VisualComponent(int winy, int winx);

        void setFrame(int winy, int winx) override;
        void render(Library& library) override;

        int getOrientation();

        void setOrientation(int orientation);
        void setRunningMaxFrequency(float freq);

    private:
        std::vector<std::wstring> visualize(int cy, int cx, std::vector<float>& data);

        void renderVisualizer(Playlist& playlist);
        void renderControls(Playlist& playlist);

        std::string getTimeStamp(double timeInSeconds);

    private:
        int _orientation;
        double _runningMaxFrequency;
};
