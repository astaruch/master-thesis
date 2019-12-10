#ifndef PHISHSCORE_SERVER_H
#define PHISHSCORE_SERVER_H

#include "options.h"
#include "database.h"

#include <string>


namespace phishscore {

class server {
public:
    explicit server(const options& opts);

    void init();
    void run();

private:
    const options& opts_;
    database db_;
    int server_sock_;
};

}

#endif //PHISHSCORE_SERVER_H
