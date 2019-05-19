#include <string>
#include <iostream>
#include "socket_client.h"

int
main()
{
    Socket_client client(8080);
    client.init_client();
    client.connect_to_server();

    std::string input;
    while (std::cin >> input) {
        if (input == "exit") {
            break;
        }
        client.send_message(input);
        client.read_message();
    }
    client.disconnect();
    return 0;
}
