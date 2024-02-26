#pragma once

#include "Parameters.hpp"
#include "Window.hpp"
#include "Library.hpp"
#include "VisualComponent.hpp"
#include "ListComponent.hpp"
#include "ConsoleComponent.hpp"
#include "Log.hpp"

class Window;

class Command {
    public:
        Command(Window& ctx, PlaylistManager& lib);
        virtual ~Command() = 0;

        virtual void execute(Parameters args) = 0;

    protected:
        VisualComponent& getVisualView();
        ListComponent& getListView();
        ConsoleComponent& getConsoleView();

    protected:
        PlaylistManager& _lib;

    private:
        Window& _ctx;
};
