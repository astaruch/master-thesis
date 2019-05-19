#ifndef PHISHING_DETECTION_SOCKET_SERVER_H
#define PHISHING_DETECTION_SOCKET_SERVER_H

#include <cstdint>
#include <netinet/in.h>

namespace phishapp {

class Socket_server {
public:
    explicit Socket_server(uint16_t port);
    ~Socket_server();

    bool
    init_server();

    bool
    wait_for_client();

    std::string
    receive_input();

    void
    send_response(const std::string& msg);

    void
    disconnect();

private:
    uint16_t m_port;
    int m_server_socket;
    int m_client_socket;
    sockaddr_in m_server_address;

};

} // namespace phishapp

#endif //PHISHING_DETECTION_SOCKET_SERVER_H
