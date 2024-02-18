#include "Command.hpp"
#include "Log.hpp"

Command::Command(Window& ctx, Library& lib) : _ctx(ctx), _lib(lib) {
    Log::append("Initializing Command Base\n"); 
}

Command::~Command() {}
