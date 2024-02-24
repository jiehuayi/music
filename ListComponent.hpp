#pragma once

#include "Component.hpp"
#include "Color.hpp"

class ListComponent : public ComponentBase {
    public:
        ListComponent();
        ListComponent(int winy, int winx);

        void setFrame(int winy, int winx) override;
        void render(Library& library) override;
        
        int getSelectedPosition();
        int getFrom();

        void setSelectedPosition(int pos);

    private:
        int _from;
        int _selectedPos;
        bool _numbered;
};
