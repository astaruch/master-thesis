#include <iostream>
#include <vector>
#include <string>

#include <spdlog/spdlog.h>
#include <pqxx/pqxx>

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
    return 0;
}
