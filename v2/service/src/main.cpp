#include <iostream>
#include <vector>
#include <string>

#include <spdlog/spdlog.h>
#include <pqxx/pqxx>
#include <Poco/URI.h>
#include <Poco/Exception.h>


using std::string;

int main() {
    spdlog::info("logger loaded");
    spdlog::info("connecting to database");

    std::string conn_string("postgresql://postgres:password@172.17.0.3/urls");

    pqxx::connection c(conn_string);
    pqxx::work txn(c);

    pqxx::row r = txn.exec1(
        "SELECT COUNT(*) FROM phishtank"
    );

    int pt_count = r[0].as<int>();
    spdlog::info("There are {} records in phishtank table.", pt_count);

    r = txn.exec1(
        "SELECT * FROM phishtank"
        "  LIMIT 1"
    );

    auto url_from_db = r[1].as<string>();
    spdlog::info("URL: {}", url_from_db);

    Poco::URI url_;
    try {
        spdlog::info("Parsing URL");
        url_ = Poco::URI(url_from_db);
    }
    catch (const Poco::Exception& ex) {
        spdlog::error("Can't parse URL: {} - {}", ex.what(), ex.message());
        return 1;
    }

    spdlog::info("Host: {}", url_.getHost());

    return 0;
}
