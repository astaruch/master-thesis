#include <netinet/in.h>

#include "spdlog/spdlog.h"

#include "socket_server.h"

namespace phishapp {

Socket_server::Socket_server(uint16_t port)
        :m_client_socket(-1),
         m_server_socket(-1),
         m_server_address(),
         m_port(port)

{
}

Socket_server::~Socket_server()
{
    disconnect();
}

bool
Socket_server::init_server()
{
    spdlog::info("initializing server");

    m_server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_server_socket == -1) {
        spdlog::error("can't create a socket");
        return false;
    }

    int options = 1;
    if (setsockopt(m_server_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
            &options, sizeof(options)) == -1) {
        spdlog::error("can't set socket options");
        return false;
    }

    m_server_address.sin_family = AF_INET;
    m_server_address.sin_addr.s_addr = INADDR_ANY;
    m_server_address.sin_port = htons(m_port);

    if (-1 == bind(m_server_socket, (struct sockaddr*) &m_server_address, sizeof(m_server_address))) {
        spdlog::error("can't bind address to the socket");
        return false;
    }

    if (listen(m_server_socket, 1) == -1) {
        spdlog::error("can't start listening");
        return false;
    }

    spdlog::info("server is listening on port {}", m_port);
    return true;
}

bool
Socket_server::wait_for_client()
{
    spdlog::info("waiting for client");
    int addrlen = sizeof(m_server_address);
    m_client_socket = accept(
            m_server_socket,
            (struct sockaddr*) &m_server_address,
            (socklen_t*) &addrlen);

    if (m_client_socket == -1) {
        spdlog::error("can't accept a connection");
        return false;
    }

    spdlog::info("client connected");
    return true;
}

std::string
Socket_server::receive_input()
{
    char buffer[1024] = {0};
    ssize_t rc = read(m_client_socket, buffer, 1024);
    if (rc <= 0) {
        spdlog::info("received nothing");
        return "";
    }
    spdlog::info("received: '{}'", buffer);
    return std::string(buffer);
}

void
Socket_server::send_response(const std::string& msg)
{
    spdlog::info("sending '{}'", msg);
    if (send(m_client_socket, msg.c_str(), msg.size(), 0) == -1)
    {
        spdlog::error("send has failed");
    }


}
void
Socket_server::disconnect()
{
    spdlog::info("closing a connection");
    close(m_client_socket);
    close(m_server_socket);
}

}
// namespace phishapp
