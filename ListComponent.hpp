#pragma once

#include "Component.hpp"
#include "Color.hpp"

class ListComponent : public ComponentBase {
    public:
        ListComponent();

        void setFrame() override;
        void render(PlaylistManager& library) override;
        
        int getSelectedPosition();
        int getFrom();

        void setSelectedPosition(int pos);

    private:
        int _from;
        int _selectedPos;
        bool _numbered;
};
