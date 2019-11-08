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

    std::FILE* output;
    if (!app._training_data_output_name.empty()) {
        output = std::fopen(app._training_data_output_name.c_str(), "w");
    } else {
        output = stdout;
    }
    if(!output) {
        std::perror("File opening failed");
        return EXIT_FAILURE;
    }

    if (app.create_training_data()) {
        std::vector<std::string> urls;

        if (!app._training_data_url.empty()) {
            urls.push_back(app._training_data_url);
            urls.push_back(app._training_data_url);
        } else if (app.training_data_stdin) {
            for (std::string url; std::getline(std::cin, url);) {
                urls.push_back(std::move(url));
            }
        }

        if (urls.empty()) {
            fmt::print(stderr, "No input URLs!\n");
            exit(1);
        }

        auto url = app._training_data_url;

        training_data td(app.verbose, app.output_include_url);
        td.set_flags(app.feature_flags(),
                     app.url_feature_flags(),
                     app.html_feature_flags(),
                     app.host_based_feature_flags());
        td.set_input_data(std::move(urls));
        td.set_label(static_cast<int>(app._training_data_class_value));
        td.set_html_features_opts(app.node_bin, app.html_script, app.htmlfeatures_bin);

        td.set_output(output);

        if (!td.create_training_data()) {
            fmt::print(stderr, "Not finished correctly.\n");
            return 1;
        }
        return 0;
    }

    auto db_string = app.get_conn_string();

    try {
        if (app.table_manipulation()) {
            spdlog::info("Connecting to a database..");
            spdlog::debug("Connection string: {}", db_string);
            database db(db_string);
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
    std::fclose(output);
    return 0;
}
