#include "CommandHandler.hpp"

CommandHandler::CommandHandler(Window& ctx) : _ctx(ctx) {}

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
    size_t start = input.find_first_not_of(" \t\n\r\f\v");
    size_t end = input.find_last_not_of(" \t\n\r\f\v");

    if (start == std::string::npos || end == std::string::npos) {
        return "";
    } else {
        return input.substr(start, end - start + 1);
    }
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

int CommandHandler::processCommand(std::string command) {
    _recent.name = command;
    execute();
    return 1;
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
    _error = "";

    size_t delimPosition = raw.find('=');
    if (delimPosition == std::string::npos) {
        // TODO: error handling
        return CMD_PARSE_FAILURE;
    } else if (delimPosition == raw.length() - 1) {
        // TODO: error handling
        return CMD_PARSE_FAILURE;
    } else if (delimPosition == 0) {
        // TODO: error handling
        return CMD_PARSE_FAILURE;
    }
    // NOTE: commands are (and should be) case sensitive
    std::string lhs = raw.substr(0, delimPosition);
    std::string rhs = raw.substr(delimPosition + 1);
    std::vector<std::string> valueList;

    size_t bStart = rhs.find('[');
    size_t bEnd = rhs.find(']');

    if (bStart == std::string::npos ||
            bEnd == std::string::npos) {
        // TODO: error handling
        return CMD_PARSE_FAILURE;
    } else if (bStart >= bEnd - 1) {
        // TODO: error handling
        return CMD_PARSE_FAILURE;
    } 

    valueList = split(rhs.substr(bStart + 1, bEnd), ',');
    _recent = {.name = lhs, .values = valueList};

    return CMD_PARSE_SUCCESS;
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
}
