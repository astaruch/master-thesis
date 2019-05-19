#include <arpa/inet.h>


#include <spdlog/spdlog.h>

#include "socket_client.h"

Socket_client::Socket_client(uint16_t port)
        :
        m_port(port),
        m_socket(-1),
        m_server_address()

{

}

Socket_client::~Socket_client()
{
    disconnect();
}

void
Socket_client::init_client()
{
    spdlog::info("initializing client");
    m_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_socket == -1) {
        spdlog::error("can't create socket");
        return;
    }
    spdlog::info("initialized");

}

void
Socket_client::connect_to_server()
{
    spdlog::info("connecting to a server");
    memset(&m_server_address, 0, sizeof(m_server_address));
    m_server_address.sin_family = AF_INET;
    m_server_address.sin_port = htons(m_port);

    if (inet_pton(AF_INET, "127.0.0.1", &m_server_address.sin_addr) < 1)
    {
        spdlog::error("invalid server address");
        return;
    }

    if (connect(m_socket, (struct sockaddr *)&m_server_address, sizeof(m_server_address)) == -1)
    {
        spdlog::error("can't connect to a server");
        return;
    }
    spdlog::info("connected");
}

void
Socket_client::send_message(const std::string& msg)
{
    spdlog::info("sending: {}", msg);
    if (send(m_socket, msg.c_str(), msg.size(), 0) == -1) {
        spdlog::error("send has failed");
        return;
    }
    spdlog::info("ok");
}
std::string
Socket_client::read_message()
{
    char buffer[1024] = {0};
    ssize_t rc = read(m_socket, buffer, 1024);
    if (rc <= 0) {
        spdlog::info("received nothing");
        return "";
    }
    spdlog::info("received: '{}'", buffer);
    return std::string(buffer);
}


void
Socket_client::disconnect()
{
    spdlog::info("closing a connection");
    close(m_socket);
}
