#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <cstring>
#include <limits>
#include <arpa/inet.h>
#include <zconf.h>

int main(int argc, const char** argv)
{
    std::string strSimpleUsage = std::string(argv[0]) + " <PORT>\n";
    if (argc < 2)
    {
        std::cerr << "Please enter port where is a phishing application listening...\n\n" << strSimpleUsage;
        return -1;
    }

    uint16_t nPort = 0;
    try
    {
        auto iPort = std::stoi(argv[1]);
        if (0 <= iPort && 65535 >= iPort)
        {
            nPort = static_cast<uint16_t>(iPort);
        } else
        {
            throw std::invalid_argument("Entered port is not in range 0-65535.");
        }
    } catch (std::invalid_argument& ex)
    {
        std::cerr << ex.what() << std::endl << strSimpleUsage;
        return -1;
    }

    std::cout << "Connecting to localhost:" << nPort << std::endl;

    struct sockaddr_in server_address{};
    int sock = 0;

    if (-1 == (sock = socket(AF_INET, SOCK_STREAM, 0)))
    {
        std::cerr << "Socket creation error. Exiting...\n";
        return -1;
    }

    memset(&server_address, 0, sizeof(server_address));

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(nPort);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (-1 == inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr))
    {
        std::cerr << "Invalid server address\n";
        return -1;
    }

    if (-1 == connect(sock, (struct sockaddr *)&server_address, sizeof(server_address)))
    {
        std::cerr << "connect failed\n";
        return -1;
    }

    char buffer[1024] = {0};
    std::string strInputURL;
    while (std::cin >> strInputURL)
    {
        if (strInputURL == "exit")
        {
            break;
        }

        if (-1 == send(sock, strInputURL.c_str(), strInputURL.size(), 0))
        {
            std::cerr << "send failed\n";
            return -1;
        }

        if (0 >= read(sock, buffer, 1024))
        {
            std::cerr << "read failed\n";
            return -1;
        }
        std::cout << buffer << std::endl;
        memset(buffer, 0, 1024);
    }

    std::cout << "close()\n";
    if (-1 == close(sock))
    {
        std::cerr << "close failed\n";
        return -1;
    }
    return 0;
}