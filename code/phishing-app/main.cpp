#include <iostream>

#include "phishtank/phishtank.h"
#include "safebrowsing.h"

int main(int argc, char* argv[])
{
    std::cout << "Hello, world\n";
    phishtank api;
    api.parse_database("../example-database.csv");
    bool is_threat = api.check_url("google.com");
    for (auto url: {"google.com", "https://www.fxdomains.com/" }) {
        std::cout << "URL " << url << " is a " << (api.check_url(url) ? "THREAT.\n" : "OK.\n");
    }
    safebrowsing google_api;
    google_api.init_cfg();
    return 0;
}