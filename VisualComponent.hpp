#pragma once

#include <string>
#include <sstream>
#include <cmath>
#include <pthread.h>

#include "Component.hpp"
#include "Color.hpp"
#include "Log.hpp"

#define V_TOP    0
#define V_RIGHT  1
#define V_BOTTOM 2
#define V_LEFT   3

struct VisualThreadArg {
    float freq;
    float maxFrequency;
    int orientation;
    int growMax;
    int baseMax;
    int b;
    bool isRB;
    std::vector<std::string>* canvas;
};

void* visualizerWorker(void* args);

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

    private:
        int _orientation;
        double _runningMaxFrequency;
};
