#include <spdlog/spdlog.h>

#include "database.h"

using std::set;
using std::string;

database::database(const std::string& conn_string)
    : _conn(conn_string), _txn(_conn)
{
    test_connection();
}

void database::add_url_columns(const std::string& table_name)
{
    _txn.exec("ALTER TABLE " + table_name + " ADD COLUMN scheme character varying");
    _txn.exec("ALTER TABLE " + table_name + " ADD COLUMN user_info character varying");
    _txn.exec("ALTER TABLE " + table_name + " ADD COLUMN host character varying");
    _txn.exec("ALTER TABLE " + table_name + " ADD COLUMN port integer");
    _txn.exec("ALTER TABLE " + table_name + " ADD COLUMN path character varying");
    _txn.exec("ALTER TABLE " + table_name + " ADD COLUMN query character varying");
    _txn.exec("ALTER TABLE " + table_name + " ADD COLUMN fragment character varying");
    _txn.commit();
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
