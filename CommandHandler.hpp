#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <functional>

#include "Window.hpp"
#include "Command.hpp"

#define CMD_PARSE_SUCCESS 1
#define CMD_PARSE_FAILURE 0

class CommandHandler {
    public:
        CommandHandler(Window& ctx);
        ~CommandHandler();

        static std::vector<std::string> split(std::string input, char delim);
        static std::string trim(const std::string input);

        void registerCommand(std::string identifier, std::function<Command* ()> cmd);

        int processCommand(std::string command);
        std::string getHandlerError();

    private:
        int parse(std::string raw);
        int execute();

    private:
        Window& _ctx;
        Parameters _recent;
        std::string _error;
        std::unordered_map<std::string, 
            std::function<Command*()>> _registeredCommands; 
};
