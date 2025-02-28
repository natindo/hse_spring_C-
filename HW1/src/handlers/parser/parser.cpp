#include "parser.h"

Request Parser::parse(const std::string& input) {
    std::string cmd, key, value;
    std::istringstream ss(input);
    ss >> cmd;
    if (cmd == "SET") {
        ss >> key >> value;
        return Request(SET, key, value);
    }
    if (cmd == "GET") {
        ss >> key;
        return Request(GET, key, "");
    }
    if (cmd == "DEL") {
        ss >> key;
        return Request(DEL, key, "");
    }
    return Request(UNKNOWN, "", "");
}
