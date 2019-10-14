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
#include "program.h"

using std::string;

int main(int argc, char* argv[]) {
    program app(argc, argv);

    app.check_general_opts();

    database db("postgresql://postgres:password@172.17.0.2/urls");
    auto records = db.get_all_records("alexa");
    spdlog::info("test");
    db.fill_db_with_url_parts("alexa", records);

    return 0;
}
