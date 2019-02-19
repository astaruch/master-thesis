#include <iostream>
#include <fstream>

#include <sqlite3.h>
#include <cxxopts.hpp>
#include <Poco/URI.h>
#include <Poco/Exception.h>

int main(int argc, char** argv)
{

    cxxopts::Options options("text_urls_to_database", " - parse list of URLs into SQL Lite database");
    options
            .positional_help("[optional args]")
            .show_positional_help();

    options.add_options()
            ("h,help", "Print help")
            ("i,input", "Input path", cxxopts::value<std::string>()->default_value(""))
            ("o,output", "Output database name", cxxopts::value<std::string>()->default_value(""));

    auto result = options.parse(argc, argv);

    if (result.count("help") || result.count("h")) {
        std::cout << options.help({"", "Group"}) << std::endl;
        return 0;
    }

    std::string input_path;
    std::string output_db;
    try {
        input_path = result["input"].as<std::string>();
        output_db = result["output"].as<std::string>();
    }
    catch (cxxopts::OptionException& ex) {
        std::cerr << "Error: " << ex.what() << "\n";
    }

    std::ifstream ifile(input_path);
    std::string url;
    std::vector<Poco::URI> urls;
    while (std::getline(ifile, url)) {
        try {
            auto parsed = Poco::URI(url);
            urls.push_back(parsed);
//            urls.emplace_back(Poco::URI(url));
        }
        catch (Poco::Exception& ex) {
            std::cout << "Poco::Exception: " << ex.what() << " " << ex.message() << std::endl;
        }
//        catch (Poco::URISyntaxException& ex) {
//            std::cout << "Poco::URISyntaxEception: " << ex.what() << " " << ex.message() << std::endl;
//        }
    }

    std::cout << "Parsed. Preparing db..." << std::endl;

    sqlite3* db;
    char* error_message;
    auto callback = [](void* not_used, int argc, char** argv, char** azColName) -> int {
        for (int i = 0; i<argc; ++i) {
            printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
        }
        printf("\n");
        return 0;
    };

    int rc = sqlite3_open(output_db.c_str(), &db);
    if (rc!=0) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << ".\n";
        return 0;
    }

    std::string sql = "DROP TABLE IF EXISTS url;";
    rc = sqlite3_exec(db, sql.c_str(), callback, nullptr, &error_message);
    if (rc!=SQLITE_OK) {
        std::cerr << "SQL error: " << error_message << "\n";
        sqlite3_free(error_message);
        return 1;
    }

    sql = R"(
CREATE TABLE url (
    id INT PRIMARY KEY NOT NULL,
    scheme TEXT,
    userinfo TEXT,
    host TEXT,
    port INT,
    path TEXT,
    query TEXT,
    fragment TEXT
)
)";

    rc = sqlite3_exec(db, sql.c_str(), callback, nullptr, &error_message);
    if (rc!=SQLITE_OK) {
        std::cerr << "SQL error: " << error_message << "\n";
        sqlite3_free(error_message);
        return 1;
    }

    rc = sqlite3_exec(db, "BEGIN TRANSACTION", nullptr, nullptr, &error_message);
    if (rc!=SQLITE_OK) {
        std::cerr << "SQL error: " << error_message << "\n";
        sqlite3_free(error_message);
        return 1;
    }

    char buffer[] = "INSERT INTO url VALUES      (?1, ?2, ?3, ?4, ?5, ?6, ?7, ?8)";
    sqlite3_stmt* stmt;

    rc = sqlite3_prepare_v2(db, buffer, -1, &stmt, nullptr);
    if (rc!=SQLITE_OK) {
        std::cerr << "Error: " << sqlite3_errmsg(db) << "\n";
        sqlite3_free(error_message);
        return 1;
    }

    for (int i = 0; i < urls.size(); ++i)
    {
        try {
            sqlite3_bind_int(stmt, 1, i);
            sqlite3_bind_text(stmt, 2, urls[i].getScheme().c_str(), static_cast<int>(urls[i].getScheme().size()),
                    SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 3, urls[i].getUserInfo().c_str(), static_cast<int>(urls[i].getUserInfo().size()),
                    SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 4, urls[i].getHost().c_str(), static_cast<int>(urls[i].getHost().size()),
                    SQLITE_TRANSIENT);
            sqlite3_bind_int(stmt, 5, static_cast<int>(urls[i].getPort()));
            sqlite3_bind_text(stmt, 6, urls[i].getPath().c_str(), static_cast<int>(urls[i].getPath().size()),
                    SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 7, urls[i].getQuery().c_str(), static_cast<int>(urls[i].getQuery().size()),
                    SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 8, urls[i].getFragment().c_str(), static_cast<int>(urls[i].getFragment().size()),
                    SQLITE_TRANSIENT);
        } catch (Poco::URISyntaxException& ex) {
            std::cout << "Poco::URISyntaxException: " << ex.what() << " " << ex.message() << std::endl;
            sqlite3_reset(stmt);
            continue;
        }

        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE) {
            std::cerr << "Commit has failed!\n";
        }

        sqlite3_reset(stmt);
    }

    rc = sqlite3_exec(db, "COMMIT TRANSACTION", nullptr, nullptr, &error_message);
    if (rc!=SQLITE_OK) {
        std::cerr << "SQL error: " << error_message << "\n";
        sqlite3_free(error_message);
        return 1;
    }

    rc = sqlite3_finalize(stmt);
    if (rc!=SQLITE_OK) {
        std::cerr << "SQL error: " << error_message << "\n";
        sqlite3_free(error_message);
        return 1;
    }
    std::cout << "Done.\n";

    return 0;
}