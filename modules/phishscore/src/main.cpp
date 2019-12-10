#include <array>
#include <iostream>
#include <set>
#include <string>
#include <vector>

#include <spdlog/spdlog.h>
#include <pqxx/pqxx>
#include <Poco/URI.h>
#include <Poco/Exception.h>
#include <nlohmann/json.hpp>

#include "database.h"
#include "program.h"
#include "feature_vector.h"
#include "training_data.h"
#include "model_checker.h"
#include "safebrowsing_api.h"
#include "env.h"
#include "server.h"
#include "check_url.h"

using json = nlohmann::json;
using options = phishscore::options;

auto unescape_inplace = [](std::string& str) -> void {
    for (size_t i = 0, end = str.size(); i < end; ++i) {
        if (str[i] == '"') {
            str.replace(i, 1, "\\\"");
            end++;
            i++;
        }
    }
};

auto unescape_copy = [](std::string str) -> std::string {
    for (size_t i = 0, end = str.size(); i < end; ++i) {
        if (str[i] == '"') {
            str.replace(i, 1, "\\\"");
            end++;
            i++;
        }
    }
    return str;
};

int main(int argc, char* argv[]) {
    phishscore::program app(argc, argv);

    app.check_options();

    const auto opts = app.get_options();

    if (opts.server != 0) {
        phishscore::server srv(opts);
        srv.init();
        srv.run();
        return 0;
    }

    if (!opts.input.url.empty()) {
        spdlog::info("Starting application to check '{}'", opts.input.url);
        database db;
        if (!db.table_exists("phish_score")) {
            db.create_table_phish_score();
        }

        auto response = phishscore::check_url(opts, opts.input.url, db);
        if (response.find("error") != response.end()) {
            spdlog::error("Error occured: {}", response.dump());
            return 1;
        }
        spdlog::info("Phishing score: {}", response["score"].get<int>());
        fmt::print("{}\n", unescape_copy(response.dump()));

        return 0;
    }

    std::FILE* output = stdout;

    if (app.create_training_data()) {
        std::vector<std::string> urls;

        if (!opts.td.url.empty()) {
            urls.push_back(opts.td.url);
        } else if (opts.td.stdin) {
            for (std::string url; std::getline(std::cin, url);) {
                urls.push_back(std::move(url));
            }
        }

        if (urls.empty()) {
            fmt::print(stderr, "No input URLs!\n");
            exit(1);
        }

        phishscore::training_data td(opts);
        td.set_input_data(std::move(urls));
        td.print_csv_training_data();

        return 0;
    }

    if (app.enable_model_checking) {
        std::vector<std::string> urls;
        if (opts.input.stdin) {
            for (std::string url; std::getline(std::cin, url);) {
                urls.push_back(url);
            }
        } else {
            urls.push_back(opts.input.url);
        }

        phishscore::training_data td(opts);
        td.set_input_data(urls);
        auto data = td.get_data_for_model();

        phishscore::model_checker_t model(opts);

        for (const auto& features_map: data) {
            json features_json(features_map);

            if (opts.verbose) fmt::print("features_json: {}\n", features_json.dump());
            auto response = model.predict(features_json);
            if (response.find("error") != response.end()) {
                spdlog::error("Error occured: {}", response.dump());
                return 1;
            }
            // fmt::print("{}\n", unescape_copy(obj.dump()));
            auto score = response["score"].get<int>();
            if (opts.verbose) fmt::print("Phishing score is: ");
            fmt::print("{}\n", score);
        }
        return 0;
    }

    const auto& db_string = opts.database.conn_string;

    try {
        if (app.table_manipulation()) {
            spdlog::info("Connecting to a database..");
            spdlog::debug("Connection string: {}", db_string);
            database db(db_string);
            if (!opts.parse_urls_to_table.empty()) {
                db.process_table_and_parse_urls(opts.parse_urls_to_table);
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
