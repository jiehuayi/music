#include <iostream>
#include <string>
#include <vector>
#include <sstream>

#define CMD_PARSE_FAILURE 1

struct Args {
    std::string name;
    std::vector<std::string> values;
};

Args parse(std::string raw);
std::string trim(const std::string input);
std::vector<std::string> split(std::string input, char delim);

void as(std::string want, std::string expected) {
    if (want == expected) {
        std::cout << "ok!\n";
    } else {
        std::cout << "assert failure: expected " << expected << "\n";
        std::cout << "got: \"" << want << "\"";
    }
}

void as(int want, int expected) {
    if (want == expected) {
        std::cout << "ok!\n";
    } else {
        std::cout << "assert failure: expected " << expected << "\n";
        std::cout << "got: \"" << want << "\"";
    }
}

void as(Args want, Args expected) {
    if (want.name == expected.name) {
        std::cout << "ok!\n";
    } else {
        std::cout << "assert failure: expected " << expected.name;
    }
    
    std::vector<std::string> v1 = want.values;
    std::vector<std::string> v2 = expected.values;
   
    // If the sizes are different, the vectors are not equal
    if (v1.size() != v2.size()) {
        std::cout << "assert failure\n";
    }

    // Iterate over each element and compare
    for (size_t i = 0; i < v1.size(); ++i) {
        if (v1[i] != v2[i]) {
            std::cout << "assert failure: got " << v1[i] << " instead of "
                << v2[i] << "\n";
        }
    }
}

int main() {
    std::string h = "hello";
    std::string s1 = "hello";
    std::string s4 = "\n\n\n\t hello";
    std::string s5 = " \n \n hello \t \t \n    \n";
    std::string s2 = " hello    ";
    std::string s6 = "hello ";
    std::string s3 = "\r \f \t hello \r\n    \t";

    as(trim(s1), h);
    as(trim(s2), h);
    as(trim(s3), h);
    as(trim(s4), h);
    as(trim(s5), h);
    as(trim(s6), h);

    std::string ss1 = "hello,world,sama";
    std::vector<std::string> v = split(ss1, ',');

    as(v[0], "hello");
    as(v[1], "world");
    as(v[2], "sama");

    std::string ss2 = "   hello, world, sama ";
    v = split(ss2, ',');

    as(v[0], "hello");
    as(v[1], "world");
    as(v[2], "sama");

    std::string ss3 = "";
    v = split(ss3, ',');

    as(v.size(), 0);

    std::string ss4  = "hello";
    v = split(ss4 , ',');

    as(v[0], "hello");

    std::string ss5 = ",";
    v = split(ss5 , ',');
    as(v[0], "");
    
    std::string ss6 = ",, ";
    v = split(ss6 , ',');
    as(v[0], "");
    as(v[1], "");
    as(v[2], ""); //strip is called
    as(v.size(), 3);

#define v std::vector<std::string>
#define s std::string

    std::string a1 = "hello=[world]";
    v v1 = v();
    v1.push_back("world");
    Args ae1 = {.name="hello", .values=v1};
    as(parse(a1), ae1); 

    std::string a2 = "hello sussy=[world    , order   , jor ] []";
    v v2 = v1;
    v2.push_back("order");
    v2.push_back("jor");
    Args ae2 = {.name="hello sussy", .values=v2};
    as(parse(a2),ae2);

    return 0;
}

std::vector<std::string> split(std::string input, char delim) {
    std::vector<std::string> pieces;
    std::string token;
    std::istringstream iss(input);
    while (std::getline(iss, token, delim)) {
        pieces.push_back(trim(token));
    }
    return pieces;
}

std::string trim(const std::string input) {
    // Find the first non-whitespace character from the beginning
    size_t start = input.find_first_not_of(" \f\t\n\r");

    // If the string is all whitespace, return an empty string
    if (start == std::string::npos) {
        return "";
    }

    // Find the last non-whitespace character
    size_t end = input.find_last_not_of(" \t\n\r");

    // Return the substring from start to end
    return input.substr(start, end - start + 1);
}

Args parse(std::string raw) {
    Args e = {.name = "", .values = std::vector<std::string>()};
    size_t delimPosition = raw.find('=');
    if (delimPosition == std::string::npos) {
        // TODO: error handling
        return e;
    } else if (delimPosition == raw.length() - 1) {
        // TODO: error handling
        return e;
    } else if (delimPosition == 0) {
        // TODO: error handling
        return e;
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
        return e;
    } else if (bStart >= bEnd - 1) {
        // TODO: error handling
        return e;
    } 

    valueList = split(rhs.substr(bStart + 1, bEnd - 1), ',');
    Args a = {.name = lhs, .values = valueList};

    return a;
}
