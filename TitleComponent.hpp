#pragma once

#include <string>

#include "Component.hpp"
#include "Clock.hpp"

#define ALERT_DEFAULT_DURATION 3000

class TitleComponent : public ComponentBase {
    public:
        TitleComponent();

        void setFrame() override;
        void render(PlaylistManager& library) override;

        void setAlert(std::string message);
        void setAlertDuration(double duration);

    private:
        void updateInternalState();

    private:
        Clock _clock;
        std::string _alertMessage;
        double _alertDuration;
        bool _alerting;
};
