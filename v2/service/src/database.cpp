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
                    spdlog::debug("parsing {}", full_url);
                    record.url_obj = Poco::URI(full_url);
                    record.ok = true;
                }
                catch (const Poco::Exception& ex) {
                    spdlog::warn("{} - {}", ex.what(), ex.message());
                    record.ok = false;
                }
            }
            records.push_back(record);
        }
        spdlog::info("count of records in table '{}': #{}", table_name, rows.size());
        spdlog::info("parsed records: {}", std::count_if(records.begin(),
            records.end(), [&](const auto& rec) { return rec.ok; }));
        return records;
    } catch (const pqxx::sql_error& ex) {
        spdlog::error("SQL problem");
        spdlog::error("{}", ex.what());
        spdlog::error("query: {}", ex.query());
    } catch (const pqxx::usage_error& ex) {
        spdlog::error("usage problem");
        spdlog::error("{}", ex.what());
    }
    return {};
}

bool database::fill_db_with_url_parts(const std::string& table_name,
    const std::vector<database::db_record>& records)
{
    spdlog::info("fill_db_with_url_parts({}, {} records)", table_name, records.size());
    prepare_update_url_parts(table_name);
    for (auto& record: records) {
        try {
            auto result = execute_update_url_parts(record.id, record.url_obj);
        } catch (const pqxx::broken_connection& ex) {
            spdlog::error("{}", ex.what());
            return false;
        }
    }
    _txn.commit();
    spdlog::info("table '{}' updated successfully", table_name);
    return true;
}

void database::add_url_columns(const std::string& table_name)
{
    try {
        _txn.exec("ALTER TABLE " + table_name + " ADD COLUMN scheme character varying");
        _txn.exec("ALTER TABLE " + table_name + " ADD COLUMN user_info character varying");
        _txn.exec("ALTER TABLE " + table_name + " ADD COLUMN host character varying");
        _txn.exec("ALTER TABLE " + table_name + " ADD COLUMN port integer");
        _txn.exec("ALTER TABLE " + table_name + " ADD COLUMN path character varying");
        _txn.exec("ALTER TABLE " + table_name + " ADD COLUMN query character varying");
        _txn.exec("ALTER TABLE " + table_name + " ADD COLUMN fragment character varying");
        _txn.commit();
    } catch (const pqxx::sql_error& ex) {
        spdlog::warn("table not changed");
        spdlog::warn("{}", ex.what());
        spdlog::warn("query: {}", ex.query());
    } catch (const std::exception& ex) {
        spdlog::error("uncaught exception: {}", ex.what());
    }
}

std::set<std::string> database::get_column_names(const std::string& table_name)
{
    pqxx::result rows = _txn.exec_params(
        "SELECT column_name FROM information_schema.columns "
        "  WHERE table_name = $1",
        table_name
    );

    set<string> schema;
    // for (const auto row: rows) {
    for (size_t i = 0; i < rows.size(); ++i) {
        schema.insert(rows[i][0].as<string>());
    }
    return schema;
}

void database::test_connection()
{
    try {
        _txn.exec1("SELECT 'test connection'");
    } catch (const std::exception& ex) {
        spdlog::error("exception: {}", ex.what());
        return;
    }
    spdlog::info("database connected");
}
