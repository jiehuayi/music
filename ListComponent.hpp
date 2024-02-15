#include "Component.hpp"

class ListComponent : public ComponentBase {
    public:
        ListComponent(int winy, int winx);

        void render(Library& library) override;

    private:
        int _from;
        int _selectedPos;
        bool _numbered;
};
