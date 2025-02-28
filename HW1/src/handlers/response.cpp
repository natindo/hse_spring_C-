#include "response.h"

#include <iostream>
#include <ostream>


void Response::setResponse(const std::string& response) {
    response_ = response;
}

std::string Response::getResponse() {
    return response_;
}
