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
#include "training_data.h"
#include "model_checker.h"

using json = nlohmann::json;

auto unescape = [](std::string& str) -> void {
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
    program app(argc, argv);

    app.check_options();

    if (!app.check_url.empty()) {
        spdlog::info("Starting application to check '{}'", app.check_url);
        database db;
        if (!db.table_exists("phish_score")) {
            db.create_table_phish_score();
        }
        auto score = db.check_phishing_score(app.check_url);
        if (score != -1) {
            json obj = json::object();
            obj[app.check_url] = score;
            fmt::print("{}\n", unescape_copy(obj.dump()));
        }

        return 0;
    }

    // Following lines were used during a development
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
        } else if (app.training_data_stdin) {
            for (std::string url; std::getline(std::cin, url);) {
                urls.push_back(std::move(url));
            }
        }

        if (urls.empty()) {
            fmt::print(stderr, "No input URLs!\n");
            exit(1);
        }

        training_data td(app.verbose, app.output_include_url, app.output_extra_values);
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

    if (app.enable_model_checking) {
        std::vector<std::string> urls;
        if (app.input_stdin) {
            for (std::string url; std::getline(std::cin, url);) {
                urls.push_back(url);
            }
        } else {
            urls.push_back(app.input_url);
        }

        training_data td(app.verbose, app.output_include_url, false);
        td.set_flags(app.feature_flags(),
                     app.url_feature_flags(),
                     app.html_feature_flags(),
                     app.host_based_feature_flags());
        td.set_input_data(urls);
        td.set_label(-1);
        td.set_html_features_opts("", "", app.htmlfeatures_bin);
        td.set_output(stdout);




        // auto unescape = [](std::string json) -> std::string {
        //     for (size_t i = 0, end = json.size(); i < end; ++i) {
        //         if (json[i] == '"') {
        //             json.replace(i, 1, "\\\"");
        //             end++;
        //             i++;
        //         }
        //     }
        //     return json;
        // };

        model_checker_t model(app.model_checker_path);

        const auto data = td.get_data_for_model();
        for (const auto& data_row: data) {
            json j(data_row);
            std::string str = j.dump();
            unescape(str);
            if (app.verbose) fmt::print("{}\n", str);
            auto phishing_score = model.predict(str);
            if (app.verbose) fmt::print("Phishing score is: ");
            fmt::print("{}\n", static_cast<int>(phishing_score * 100));
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
