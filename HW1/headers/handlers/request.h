#ifndef REQUEST_H
#define REQUEST_H

#include <string>

enum RequestType {
    UNKNOWN,
    SET,
    GET,
    DEL
};

class Request {
public:
    Request() = delete;
    Request(const RequestType type, const std::string &key, const std::string &value)
    : key_(key), value_(value), type_(type) {};
    RequestType getType();
    std::string getKey();
    std::string getValue();
    ~Request() = default;
private:
    std::string key_;
    std::string value_;
    RequestType type_;
    void parseRequest(std::string request);
};

#endif //REQUEST_H
