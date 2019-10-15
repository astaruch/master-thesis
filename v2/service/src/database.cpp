#include <algorithm>

#include <pqxx/except>
#include <pqxx/pqxx>
#include <spdlog/spdlog.h>

#include "database.h"

using std::set;
using std::string;

database::database(const std::string& conn_string)
    : _conn(conn_string)
{
    test_connection();
}

void database::prepare_update_url_parts(const std::string& table_name, pqxx::work& txn)
{
    _conn.prepare(
        "update_url_parts",
        "UPDATE " + txn.esc(table_name) + " SET "
        "  scheme = $2::text, "
        "  user_info = $3::text, "
        "  host = $4::text, "
        "  port = $5::integer, "
        "  path = $6::text, "
        "  query = $7::text, "
        "  fragment = $8::text "
        "WHERE id = $1");
}

pqxx::result database::execute_update_url_parts(pqxx::work& txn, int id,
    const std::string* scheme,
    const std::string* user_info,
    const std::string* host,
    unsigned int* port,
    const std::string* path,
    const std::string* query,
    const std::string* fragment)
{
    return txn.exec_prepared("update_url_parts",
        id,
        scheme->empty() ? nullptr : scheme->c_str(),
        user_info->empty() ? nullptr : user_info->c_str(),
        host->empty() ? nullptr : host->c_str(),
        *port == 0 ? nullptr : std::to_string(*port).c_str(),
        path->empty() ? nullptr : path->c_str(),
        query->empty() ? nullptr : query->c_str(),
        fragment->empty() ? nullptr : fragment->c_str());
}

pqxx::result database::execute_update_url_parts(pqxx::work& txn, int id, const Poco::URI& url)
{
    auto scheme = url.getScheme();
    auto user_info = url.getUserInfo();
    auto host = url.getHost();
    unsigned port = url.getPort();
    auto path = url.getPath();
    auto query = url.getRawQuery();
    auto fragment = url.getFragment();
    return execute_update_url_parts(txn, id,
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
    pqxx::work txn(_conn);
    try {
        spdlog::info("Obtaining all records from table '{}'. Parsing on the fly", table_name);
        auto rows = txn.exec("SELECT id, url FROM " + txn.esc(table_name));
        std::vector<db_record> records;
        for (const auto& row: rows) {
            auto id = row[0].as<int>();
            auto full_url = row[1].as<string>();
            if (!full_url.empty()) {
                db_record record;
                record.id = id;
                record.full_url = full_url;
                try {
                    record.url_obj = Poco::URI(full_url);
                    record.ok = true;
                } catch (const Poco::SyntaxException& ex) {
                    spdlog::error("Malformed URL: {}", full_url);
                    record.url_obj = Poco::URI();
                    record.ok = false;
                }
                records.push_back(record);
            }
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
    std::vector<database::db_record>& records)
{
    pqxx::work txn(_conn);
    spdlog::info("Inserting parsed records into database");
    prepare_update_url_parts(table_name, txn);
    for (auto& record: records) {
        try {
            execute_update_url_parts(txn, record.id, record.url_obj);
        } catch (const pqxx::data_exception& ex) {
            spdlog::error("{}", ex.what());
            spdlog::error("Malformed data (error in encoding in Poco::URI::Parse())");
            spdlog::error("Reoving this URL from records: {}", record.full_url);
            debug_record(record);
            record.url_obj = Poco::URI();
            throw pqxx::transaction_rollback("Removing malformed URL");
        }
    }
    txn.commit();
    return true;
}

void database::debug_record(const database::db_record& record)
{
    spdlog::debug("url = {}", record.url_obj.toString());
    spdlog::debug("scheme = {}", record.url_obj.getScheme());
    spdlog::debug("user_info = {}", record.url_obj.getUserInfo());
    spdlog::debug("host = {}", record.url_obj.getHost());
    spdlog::debug("port = {}", record.url_obj.getPort());
    spdlog::debug("path = {}", record.url_obj.getPath());
    spdlog::debug("raw query = {}", record.url_obj.getRawQuery());
    spdlog::debug("query = {}", record.url_obj.getQuery());
    spdlog::debug("fragment = {}", record.url_obj.getFragment());
}

void database::add_url_columns(const std::string& table_name)
{
    pqxx::work txn(_conn);
    try {
        spdlog::info("Adding columns 'scheme', 'user_info', 'host', 'port', 'path', 'query', 'fragment'");
        txn.exec("ALTER TABLE " + txn.esc(table_name) + " ADD COLUMN IF NOT EXISTS scheme VARCHAR");
        txn.exec("ALTER TABLE " + txn.esc(table_name) + " ADD COLUMN IF NOT EXISTS user_info VARCHAR");
        txn.exec("ALTER TABLE " + txn.esc(table_name) + " ADD COLUMN IF NOT EXISTS host VARCHAR");
        txn.exec("ALTER TABLE " + txn.esc(table_name) + " ADD COLUMN IF NOT EXISTS port INTEGER");
        txn.exec("ALTER TABLE " + txn.esc(table_name) + " ADD COLUMN IF NOT EXISTS path VARCHAR");
        txn.exec("ALTER TABLE " + txn.esc(table_name) + " ADD COLUMN IF NOT EXISTS query VARCHAR");
        txn.exec("ALTER TABLE " + txn.esc(table_name) + " ADD COLUMN IF NOT EXISTS fragment VARCHAR");
        txn.commit();
    } catch (const pqxx::sql_error& ex) {
        spdlog::error("Error: {}", ex.what());
        spdlog::error("Query: {}", ex.query());
    }
}

std::set<std::string> database::get_column_names(const std::string& table_name)
{
    pqxx::work txn(_conn);
    auto rows = txn.exec_params(
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
    pqxx::work txn(_conn);
    try {
        txn.exec1("SELECT '1'");
    } catch (const std::exception& ex) {
        spdlog::error("There is a problem with connection: {}", ex.what());
        exit(1);
    }
    spdlog::info("Connected");
}

void database::process_table_and_parse_urls(const std::string& table_name)
{
    spdlog::info("Checking if table '{}' exists", table_name); // TODO: change to debug after implementation
    if (table_name.empty()) {
        spdlog::error("Invalid empty name. Stopping");
        return;
    }
    try {
        pqxx::work txn(_conn);
        auto str = txn.exec_params1("SELECT to_regclass($1)", table_name)[0].as<string>();
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

    // try several times to insert data into db, each time we will modify them
    int tries = 1;
    int max_tries = 10;
    const auto& update_callback = [this, &table_name, &records, &tries]() {
        spdlog::info("Try {}", tries++);
        return fill_db_with_url_parts(table_name, records);
    };
    bool updated_ok = pqxx::perform(update_callback, max_tries);
    if (updated_ok) {
        spdlog::info("OK");
    } else {
        spdlog::error("ERROR. Can't insert records after {} tries", max_tries);
    }
}
