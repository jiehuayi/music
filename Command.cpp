#include "Command.hpp"

Command::Command(Window& ctx, Library& lib) : _ctx(ctx), _lib(lib) {
    Log::append("Initializing Command Base\n"); 
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
