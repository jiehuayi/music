#include "Command.hpp"
#include "Log.hpp"

Command::Command(Window& ctx) : _ctx(ctx) {
    Log::append("Initializing Command Base\n"); 
}
