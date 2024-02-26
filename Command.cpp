#include "Command.hpp"

Command::Command(Window& ctx, PlaylistManager& lib) : _lib(lib), _ctx(ctx) {
    TRACE1("Initializing Base Command");
}

Command::~Command() {}

VisualComponent& Command::getVisualView() { 
    return _ctx._visualView; 
}

ListComponent& Command::getListView() { 
    return _ctx._listView; 
}
ConsoleComponent& Command::getConsoleView() { 
    return _ctx._consoleView;
}
