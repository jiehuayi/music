#include "CommandHandler.hpp"

CommandHandler::CommandHandler() {
    _recent = {.name = "", .values = {}};
    _error = "";
}

CommandHandler::~CommandHandler() {}

std::vector<std::string>
CommandHandler::split(std::string input, char delim) {
    std::vector<std::string> pieces;
    std::string token;
    std::istringstream iss(input);
    while (std::getline(iss, token, delim)) {
        pieces.push_back(token);
    }
    return pieces;
}

std::string CommandHandler::trim(const std::string input) {
    size_t start = input.find_first_not_of(" \f\t\n\r");

    if (start == std::string::npos) {
        return "";
    }

    size_t end = input.find_last_not_of(" \t\n\r");
    return input.substr(start, end - start + 1);
}

int CommandHandler::registerCommand(std::string identifier, 
        std::function<Command* ()> cmd) {
    if (cmd == nullptr) {
        return 1;
    }

    if (_registeredCommands.find(identifier) == _registeredCommands.end()) {
        _registeredCommands[identifier] = cmd; 
        return 0;
    }

    // TODO: error handling
    return 1;
}

int CommandHandler::registerAlias(std::string identifier, std::string aliasList) {
    std::vector<std::string> aliases = split(aliasList, ',');
    for (const auto& alias : aliases) {
        if (alias == "") {
            continue;
        }

        if (_registeredAliases.find(alias) == _registeredAliases.end()) {
            _registeredAliases[alias] = identifier;
        }
    }
    
    // TODO: error handling
    return 1;
}

int CommandHandler::processCommand(std::string command) {
    auto fullCommandIter = _registeredAliases.find(command);

    // The entered command may be a alias, if not, treat it as a real command
    if (fullCommandIter != _registeredAliases.end()) {
        _recent.name = fullCommandIter->second;
    } else {
        _recent.name = command;
    }
    parse(command);
    return execute();
}

int CommandHandler::processRecentCommand() {
    return processCommand(_recent.name); 
}

std::string CommandHandler::getHandlerError() {
    return _error;
}

// Command structure:
// <command_name>=[<value_list>]
// 
// Value list structure:
// [<value1>,<value2>, ... ,<valueN>]
int CommandHandler::parse(std::string raw) {
    std::string clean = trim(raw);
    std::string name;
    std::string values;

    try {
        name = _parser.parseCommandName(clean);
        values = _parser.parseCommandValueList(clean);
    } catch (const std::runtime_error& err) {
        _error = err.what();
        TRACE1(_error);
    }

    if (values == "") {
        _recent.name = name;
        _recent.values = {};
        return 0;
    }

    // TODO 

}

int CommandHandler::execute() {
    if (_registeredCommands.find(_recent.name) == _registeredCommands.end()) {
        return 1;
    }

    Command* c = _registeredCommands[_recent.name]();

    if (c == nullptr) {
        return 2;
    }

    c->execute(_recent);
    delete c;

    return 0;
}
