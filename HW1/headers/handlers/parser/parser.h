#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <sstream>
#include "request.h"

class Parser {
public:
    Request static parse(const std::string& input);
};

#endif //PARSER_H
