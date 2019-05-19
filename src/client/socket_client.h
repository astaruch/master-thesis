#ifndef PHISHING_DETECTION_SOCKET_CLIENT_H
#define PHISHING_DETECTION_SOCKET_CLIENT_H

#include <netinet/in.h>

class Socket_client {
public:
    explicit Socket_client(uint16_t port);
    ~Socket_client();

    void
    init_client();

    void
    connect_to_server();

    void
    send_message(const std::string& msg);

    std::string
    read_message();

    void
    disconnect();
private:
    uint16_t m_port;
    int m_socket;
    sockaddr_in m_server_address;

};

#endif //PHISHING_DETECTION_SOCKET_CLIENT_H
