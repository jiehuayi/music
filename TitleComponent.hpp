#pragma once

#include "Component.hpp"

class TitleComponent : public ComponentBase {
    public:
        TitleComponent();

        void setFrame() override;
        void render(PlaylistManager& library) override;
};
