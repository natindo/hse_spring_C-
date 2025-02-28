#ifndef RESPONSE_H
#define RESPONSE_H

#include <string>

class Response {
public:
    Response() = default;
    std::string getResponse();
    void setResponse(const std::string& response);
private:
    std::string response_;
};

#endif //RESPONSE_H
