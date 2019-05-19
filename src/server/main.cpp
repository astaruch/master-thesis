#include <iostream>

#include <spdlog/spdlog.h>

#include "libs/config.h"
#include "libs/database.h"

#include "src/server/url_test.h"
#include "src/server/socket_server.h"

using namespace phishapp;

int
main()
{
    spdlog::info("starting the app");
    Config::set_path("config.yaml");

    auto db = Database();
    db.create_table("url_test", true);

    auto server = Socket_server(8080);
    server.init_server();
    server.wait_for_client();

    std::string url = server.receive_input();
    while (!url.empty()) {
        Url_test url_test(url);
        int phish_score = url_test.do_tests();
        server.send_response(fmt::format("Phishing score is {}. URL: {}", phish_score, url));
        url = server.receive_input();
    }

    server.disconnect();
    Config::save();
    return 0;
}

