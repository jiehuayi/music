#include <string>
#include <sstream>

#include "Component.hpp"

class ConsoleComponent : public ComponentBase {
    public:
        ConsoleComponent();
        ConsoleComponent(int winy, int winx);

        void render(Library& library) override;

        std::string getInputBuffer();

        void appendInputBuffer(char inputCharacter);
        void setInputBuffer(std::string input);

    private:
        bool isValidCommandChar(char inputCharacter);

    private:
        std::stringstream _inputBuffer;
};
