#include "server.h"

#include "check_url.h"

#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>

#include <stdio.h> // standard input and output library
#include <stdlib.h> // this includes functions regarding memory allocation
#include <string.h> // contains string functions
#include <errno.h> //It defines macros for reporting and retrieving error conditions through error codes
#include <time.h> //contains various functions for manipulating date and time
#include <unistd.h> //contains various constants
#include <sys/types.h> //contains a number of basic derived types that should be used whenever appropriate
#include <arpa/inet.h> // defines in_addr structure
#include <sys/socket.h> // for socket creation
#include <netinet/in.h> //contains con

using nlohmann::json;

namespace phishscore {

server::server(const options& opts)
    : opts_(opts)
    , db_(database())
{
}

void server::init()
{
    if (!db_.table_exists("phish_score")) {
        db_.create_table_phish_score();
    }

	struct sockaddr_in server_address;
	server_sock_ = socket(AF_INET, SOCK_STREAM, 0);
    int flags = 1;
    int ret = setsockopt(server_sock_, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &flags, sizeof(flags));
    if (ret == -1) {
        spdlog::error("can't set socket options");
        exit(1);
    }
	memset(&server_address, '0', sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = INADDR_ANY;
	server_address.sin_port = htons(opts_.server);
	bind(server_sock_, reinterpret_cast<struct sockaddr*>(&server_address) , sizeof(server_address));
	listen(server_sock_ , 20);
}

void server::run()
{
	char buffer[1025] = {0};
    int client_sock = 0;
    while(1) {
		client_sock = accept(server_sock_, (struct sockaddr*)NULL, NULL);
        spdlog::info("Client connected");
        ssize_t rc = read(client_sock, buffer, 1024);
        if (rc <= 0) {
            spdlog::warn("Received empty request");
            close(client_sock);
            continue;
        }
        spdlog::info("Received {}", buffer);
        json request;
        try {
            request = json::parse(buffer);
        } catch (...) {
            spdlog::error("Invalid JSON input");
            close(server_sock_);
            exit(1);
        }
        std::string url = request["url"].get<std::string>();
        json response = phishscore::check_url(opts_, url, db_);
        spdlog::info("Sending: {}", response.dump());
        std::string response_str = response.dump();
        write(client_sock, response_str.c_str(), response_str.size());
        spdlog::info("Client disconnected");
        close(client_sock);
        sleep(1);
     }
}

} // namespace phishscore
