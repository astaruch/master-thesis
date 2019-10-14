#include <algorithm>

#include <pqxx/except>
#include <pqxx/pqxx>
#include <spdlog/spdlog.h>

#include "database.h"

using std::set;
using std::string;

database::database(const std::string& conn_string)
    : _conn(conn_string), _txn(_conn)
{
    test_connection();
}

void database::prepare_update_url_parts(const std::string& table_name)
{
    _conn.prepare(
        "update_url_parts",
        "UPDATE " + _txn.esc(table_name) + " SET "
        "  scheme = $2, "
        "  user_info = $3, "
        "  host = $4, "
        "  port = $5, "
        "  path = $6, "
        "  query = $7, "
        "  fragment = $8 "
        "WHERE id = $1");
}

pqxx::result database::execute_update_url_parts(int id,
    const std::string* scheme,
    const std::string* user_info,
    const std::string* host,
    unsigned int* port,
    const std::string* path,
    const std::string* query,
    const std::string* fragment)
{
    return _txn.exec_prepared("update_url_parts",
        id,
        scheme->empty() ? nullptr : scheme->c_str(),
        user_info->empty() ? nullptr : user_info->c_str(),
        host->empty() ? nullptr : host->c_str(),
        *port == 0 ? nullptr : std::to_string(*port).c_str(),
        path->empty() ? nullptr : path->c_str(),
        query->empty() ? nullptr : query->c_str(),
        fragment->empty() ? nullptr : fragment->c_str());
}

pqxx::result database::execute_update_url_parts(int id, const Poco::URI& url)
{
    auto scheme = url.getScheme();
    auto user_info = url.getUserInfo();
    auto host = url.getHost();
    unsigned port = url.getPort();
    auto path = url.getPath();
    auto query = url.getQuery();
    auto fragment = url.getFragment();
    return execute_update_url_parts(id,
        &scheme,
        &user_info,
        &host,
        &port,
        &path,
        &query,
        &fragment);
}

std::vector<database::db_record> database::get_all_records(const std::string& table_name)
{
    try {
        spdlog::info("Obtaining all records from table '{}'. Parsing on the fly", table_name);
        auto rows = _txn.exec("SELECT id, url FROM " + _txn.esc(table_name));
        std::vector<db_record> records;
        for (const auto& row: rows) {
            auto id = row[0].as<int>();
            auto full_url = row[1].as<string>();
            db_record record;
            if (!full_url.empty()) {
                record.id = id;
                record.full_url = full_url;
                try {
                    record.url_obj = Poco::URI(full_url);
                    record.ok = true;
                } catch (const Poco::Exception& ex) {
                    spdlog::error("Can't parse an URL: {}", full_url);
                    spdlog::error("Error: {}", ex.what());
                    spdlog::error("Message: {}", ex.message());
                    record.ok = false;
                }
            }
            records.push_back(record);
        }
        spdlog::info("Number of URLs in table: {}", rows.size());
        spdlog::info("Number of parsed URLs: {}", std::count_if(records.begin(),
            records.end(), [&](const auto& rec) { return rec.ok; }));
        return records;
    } catch (const pqxx::sql_error& ex) {
        spdlog::error("Error: {}", ex.what());
        spdlog::error("Query: {}", ex.query());
    } catch (const pqxx::usage_error& ex) {
        spdlog::error("Error: {}", ex.what());
    }
    return {};
}

bool database::fill_db_with_url_parts(const std::string& table_name,
    const std::vector<database::db_record>& records)
{
    spdlog::info("Inserting parsed records into database");
    prepare_update_url_parts(table_name);
    try {
        for (auto& record: records) {
            execute_update_url_parts(record.id, record.url_obj);
        }
    } catch (const std::exception& ex) {
        spdlog::error("Error: {}", ex.what());
        return false;
    }
    _txn.commit();
    spdlog::info("OK");
    return true;
}

void database::add_url_columns(const std::string& table_name)
{
    try {
        spdlog::info("Adding columns 'scheme', 'user_info', 'host', 'port', 'path', 'query', 'fragment'");
        _txn.exec("ALTER TABLE " + _txn.esc(table_name) + " ADD COLUMN IF NOT EXISTS scheme VARCHAR");
        _txn.exec("ALTER TABLE " + _txn.esc(table_name) + " ADD COLUMN IF NOT EXISTS user_info VARCHAR");
        _txn.exec("ALTER TABLE " + _txn.esc(table_name) + " ADD COLUMN IF NOT EXISTS host VARCHAR");
        _txn.exec("ALTER TABLE " + _txn.esc(table_name) + " ADD COLUMN IF NOT EXISTS port INTEGER");
        _txn.exec("ALTER TABLE " + _txn.esc(table_name) + " ADD COLUMN IF NOT EXISTS path VARCHAR");
        _txn.exec("ALTER TABLE " + _txn.esc(table_name) + " ADD COLUMN IF NOT EXISTS query VARCHAR");
        _txn.exec("ALTER TABLE " + _txn.esc(table_name) + " ADD COLUMN IF NOT EXISTS fragment VARCHAR");
        _txn.commit();
    } catch (const pqxx::sql_error& ex) {
        spdlog::error("Error: {}", ex.what());
        spdlog::error("Query: {}", ex.query());
    }
}

std::set<std::string> database::get_column_names(const std::string& table_name)
{
    auto rows = _txn.exec_params(
        "SELECT column_name FROM information_schema.columns "
        "  WHERE table_name = $1",
        table_name
    );

    std::set<std::string> schema;
    for (const auto& row: rows) {
        schema.insert(row[0].as<string>());
    }
    return schema;
}

void database::test_connection()
{
    try {
        _txn.exec1("SELECT '1'");
    } catch (const std::exception& ex) {
        spdlog::error("There is a problem with connection: {}", ex.what());
        exit(1);
    }
    spdlog::info("Connected");
}

void database::process_table_and_parse_urls(const std::string& table_name)
{
    spdlog::info("Checking whether table '{}' exists", table_name); // TODO: change to debug after implementation
    if (table_name.empty()) {
        spdlog::error("Invalid empty name. Stopping");
        return;
    }
    try {
        auto str = _txn.exec_params1("SELECT to_regclass($1)", table_name)[0].as<string>();
        spdlog::info("Table '{}' exists", str);
    } catch (const pqxx::conversion_error& ex) {
        spdlog::error("Table '{}' doesn't exist", table_name);
        return;
    }

    spdlog::info("Checking if table has needed input columns: 'id', 'url'");
    {
        auto columns = get_column_names(table_name);
        if (columns.count("id") == 0) {
            spdlog::error("Missing column 'id'");
            return;
        }
        if (columns.count("url") == 0) {
            spdlog::error("Missing column 'url'");
            return;
        }
    }
    spdlog::info("OK");

    spdlog::info("Checking if table has columns for URL parts");
    {
        auto columns = get_column_names(table_name);
        if (columns.count("scheme") == 0 || columns.count("user_info") == 0 ||
            columns.count("host") == 0 || columns.count("port") == 0 ||
            columns.count("path") == 0 || columns.count("query") == 0 ||
            columns.count("fragment") == 0)
        {
            spdlog::info("Missing some columns");
            add_url_columns(table_name);
        }
    }
    spdlog::info("OK");

    auto records = get_all_records(table_name);
    fill_db_with_url_parts(table_name, records);

}
