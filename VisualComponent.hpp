#include <string>
#include <sstream>
#include <ncurses.h>

#include "Component.hpp"
#include "Log.hpp"

#define V_TOP    0
#define V_RIGHT  1
#define V_BOTTOM 2
#define V_LEFT   3

class VisualComponent : public ComponentBase {
    public:
       VisualComponent(int winy, int winx);

       void render(Library& library) override;

       std::string getTimeStamp(double timeInSeconds);

       std::vector<std::string> visualize(int cx, int cy, std::vector<float> data);

    private:
        int _orientation;
        int _runningMaxFrequency;
};
