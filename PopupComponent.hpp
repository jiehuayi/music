#include "Component.hpp"

class PopupComponent : public ComponentBase {
    public:
        PopupComponent();
        PopupComponent(int winy, int winx);
    
        void setFrame(int winy, int winx) override;
        void render(Library& library) override;
};
