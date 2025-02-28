#ifndef APP_H
#define APP_H

#include "db.h"
#include "parser.h"
#include "request.h"
#include "response.h"

class App {
public:
    App() = default;
    int run();
    Database db_;
};

#endif //APP_H
