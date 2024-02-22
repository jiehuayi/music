#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <sstream>

#define PARSE_OK                            0
#define PARSE_ERROR_ILLEGAL_CHAR_NAME       1000
#define PARSE_ERROR_ILLEGAL_CHAR_VALUES     1001 
#define PARSE_ERROR_ILLEGAL_BRACKETS        1002

class CommandParser {
    public:
        CommandParser();
        ~CommandParser();

        static std::vector<std::string> split(std::string input, char delim);
        static std::string trim(const std::string input);

        std::string parseCommandName(std::string& command);
        std::string parseCommandValueList(std::string& command);

        int commandParserStatus();

    private:
        int _status;
};
