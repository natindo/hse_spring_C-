#include "request.h"

RequestType Request::getType() {
     return type_;
}

std::string Request::getKey() {
     return key_;
}

std::string Request::getValue() {
     return value_;
}