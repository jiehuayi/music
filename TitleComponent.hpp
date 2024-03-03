#pragma once

#include "Component.hpp"
#include "Clock.hpp"

class TitleComponent : public ComponentBase {
    public:
        TitleComponent();

        void setFrame() override;
        void render(PlaylistManager& library) override;

    private:
        Clock _clock;
};
