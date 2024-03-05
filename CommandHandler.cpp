#include "CommandHandler.hpp"

CommandHandler::CommandHandler() {
    _recent = {.name = "", .values = {}};
    _error = "";
}

CommandHandler::~CommandHandler() {}

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
    std::vector<std::string> aliases = _parser.split(aliasList, ',');
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

    if (parse(command)) {
        return 1;
    }

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
    std::string clean = _parser.trim(raw);
    std::string name;
    std::string values;

    try {
        name = _parser.parseCommandName(clean);
        values = _parser.parseCommandValueList(clean);
    } catch (const std::runtime_error& err) {
        _recent.name = "err";
        _recent.values = { std::string(err.what()) };
        execute();
        TRACE1(_error);
        return 1;
    }

    _recent.name = name;

    if (values == "") {
        _recent.values = {};
        return 0;
    }
    std::vector<std::string> valueList = _parser.split(values, ',');
    std::for_each(valueList.begin(), valueList.end(), _parser.trim);
    _recent.values = valueList;
    return 0;
}

int CommandHandler::execute() {
    if (_registeredCommands.find(_recent.name) == _registeredCommands.end()) {
        signalError("ERROR: command named \"" + _recent.name + "\" not found.");
        return 1;
    }

    Command* c = _registeredCommands[_recent.name]();

    if (c == nullptr) {
        signalError("ERROR: command named \"" + _recent.name + "\" is null");
        return 2;
    }

    c->execute(_recent);
    delete c;

    return 0;
}

void CommandHandler::signalError(std::string message) {
    Command* e = _registeredCommands["err"]();
    e->execute({ .name="err", .values={ std::string(message) } });
    delete e;

}
