#include <array>
#include <iostream>
#include <set>
#include <string>
#include <vector>

#include <spdlog/spdlog.h>
#include <pqxx/pqxx>
#include <Poco/URI.h>
#include <Poco/Exception.h>

#include "database.h"

using std::string;

int main() {
    spdlog::info("logger loaded");

    database db("postgresql://postgres:password@172.17.0.2/urls");
    db.get_all_records("alexa");

    return 0;
}
