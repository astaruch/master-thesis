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
#include "training_data.h"

int main(int argc, char* argv[]) {
    program app(argc, argv);

    app.check_options();

    auto features_to_check = app.feature_flags();

    if (app.create_training_data()) {
        auto url = app.training_data_url();
        auto value = app.training_data_class_value();
        auto output_name = app.training_data_output_name();

        // TODO: change to accept stdin/input file
        std::vector<std::string> urls{url};
        training_data td;
        td.set_feature_flags(features_to_check);
        td.set_input_data(urls);
        td.set_label(value);
        td.set_output_name(output_name);
        if (!td.create_training_data()) {
            spdlog::error("Not finished correctly.");
            return 1;
        }
        return 0;
    }

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
