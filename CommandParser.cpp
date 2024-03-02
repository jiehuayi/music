#include "CommandParser.hpp"

CommandParser::CommandParser() {}
CommandParser::~CommandParser() {}

std::vector<std::string>
CommandParser::split(std::string input, char delim) {
    std::vector<std::string> pieces;
    std::string token;
    std::istringstream iss(input);
    while (std::getline(iss, token, delim)) {
        pieces.push_back(token);
    }
    return pieces;
}

std::string CommandParser::trim(const std::string input) {
    size_t start = input.find_first_not_of(" \f\t\n\r");

    if (start == std::string::npos) {
        return "";
    }

    size_t end = input.find_last_not_of(" \t\n\r");
    return input.substr(start, end - start + 1);
}

std::string CommandParser::parseCommandName(std::string& command) {
    _status = PARSE_OK;
    
    std::string rv;
    size_t nameEnd = command.find("=");

    if (nameEnd == std::string::npos) {
        rv = trim(command);
    } else {
        rv = trim(command.substr(0, nameEnd));
    }

    size_t wSpace = rv.find_first_of(" \f\t\n\r");
    if (wSpace < nameEnd) {
        throw std::runtime_error("ERROR: command name should not contain spaces");
    }

    size_t illChar = rv.find_first_of("[]");
    if (illChar < nameEnd) {
        throw std::runtime_error("ERROR: command name contains illegal characters");
    }

    return rv;
}

std::string
CommandParser::parseCommandValueList(std::string& command) {
    std::string rv = "";
    size_t operatorStart = command.find("=");
    size_t valuesStart = command.find("[") + 1;
    size_t valuesEnd = command.find("]");

    if (operatorStart == std::string::npos) {
        return rv;
    }

    if (valuesStart == std::string::npos || valuesEnd == std::string::npos) {
        throw std::runtime_error("ERROR: no matching brackets \
                in argument-based command");
    }

    if (!(valuesStart > operatorStart)) {
        throw std::runtime_error("ERROR: illegal character in value list");
    }
    
    return command.substr(valuesStart, valuesStart - valuesEnd);
}

int CommandParser::commandParserStatus() {
    return _status;
}
