#include "app.h"
#include "response.h"
#include "request.h"

#include <iostream>
#include <parser.h>
#include <string>

int App::run() {
    while (true) {
        std::string input;
        if (!std::getline(std::cin, input)) {
            break;
        }
        Response resp;
        switch (Request req = Parser::parse(input); req.getType()) {
            case UNKNOWN:
                break;
            case SET:
                if (db_.set(req.getKey(),req.getValue())) {
                    resp.setResponse("ok");
                }
                break;
            case GET:
                resp.setResponse(db_.get(req.getKey()));
                break;
            case DEL:
                if (db_.del(req.getKey())) {
                    resp.setResponse("ok");
                } else {
                    resp.setResponse("not found");
                }
                break;
        }
        std::cout << resp.getResponse() << std::endl;
    }
    return 0;
}
