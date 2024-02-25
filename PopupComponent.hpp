#include "Component.hpp"

class PopupComponent : public ComponentBase {
    public:
        PopupComponent();
    
        void setFrame() override;
        void render(Library& library) override;
};
