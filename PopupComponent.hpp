#include "Component.hpp"

class PopupComponent : public ComponentBase {
    public:
        PopupComponent();
        PopupComponent(int winy, int winx);

        void render(Library& library) override;
};
