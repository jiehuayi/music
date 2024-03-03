#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <functional>

#include "Parameters.hpp"
#include "Command.hpp"
#include "CommandParser.hpp"

#define CMD_PARSE_SUCCESS 1
#define CMD_PARSE_FAILURE 0

class Window;
class Command;

class CommandHandler {
    public:
        CommandHandler();
        ~CommandHandler();

        int registerCommand(std::string identifier, std::function<Command* ()> cmd);
        int registerAlias(std::string identifier, std::string alias);

        int processCommand(std::string command);
        int processRecentCommand();

        std::string getHandlerError();

    private:
        int parse(std::string raw);
        int execute();

    private:
        CommandParser _parser;
        Parameters _recent;
        std::string _error;
        std::unordered_map<std::string, 
            std::function<Command*()>> _registeredCommands; 
        std::unordered_map<std::string, std::string> _registeredAliases;
};
