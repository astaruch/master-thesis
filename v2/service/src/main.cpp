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

    app.check_general_opts();

    auto db_string = app.get_conn_string();

    try {
        database db(db_string);

        // auto records = db.get_all_records("alexa");
        // db.fill_db_with_url_parts("alexa", records);
    } catch (const pqxx::broken_connection& ex) {
        fmt::print(stderr, "There was an error with connecting to a database.\n");
        fmt::print(stderr, "Maybe wrong connection string/parameters for a database?\n\n");
        fmt::print(stderr, "Error: {}\n", ex.what());
        exit(1);
    }
    return 0;
}
