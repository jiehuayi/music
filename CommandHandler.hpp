#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>

#include "Window.hpp"
#include "Command.hpp"

struct Parameters {
    std::string name;
    std::vector<std::string> values; 
};

class CommandHandler {
    public:
        CommandHandler(Window& ctx);
        ~CommandHandler();

        void registerCommand(std::string identifier, Command command);

        int processCommand(std::string command);
        void getHandlerError();

    private:
        int parse();
        int execute();

    private:
        Window& _ctx;
        std::string _error;
        std::unordered_map<std::string, 
            std::function<Command*()>> _registeredCommands; 
};
