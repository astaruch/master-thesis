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

int main(int argc, char* argv[]) {
    program app(argc, argv);

    app.check_options();

    auto db_string = app.get_conn_string();

    try {
        spdlog::info("Connecting to a database..");
        spdlog::debug("Connection string: {}", db_string);
        database db(db_string);

        if (app.table_manipulation()) {
            if (app.parse_urls()) {
                db.process_table_and_parse_urls(app.table_name());
                return 0;
            }
        }
    } catch (const pqxx::broken_connection& ex) {
        fmt::print(stderr, "There was an error with connecting to a database.\n");
        fmt::print(stderr, "Maybe wrong connection string/parameters for a database?\n\n");
        fmt::print(stderr, "Error: {}\n", ex.what());
        exit(1);
    }
    return 0;
}
