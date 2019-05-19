#include "spdlog/spdlog.h"

#include "config.h"
#include "database.h"

namespace phishapp {

Database::Database()
{
    auto conn_string = Config::conn_string();
    spdlog::info("connecting to '{}'", conn_string);
    try {
        m_conn = tao::pq::connection::create(conn_string);
    }
    catch (const std::exception& ex) {
        spdlog::error("{}", ex.what());
        throw ex;
    }
}

bool
Database::table_exists(const std::string& table_name, const std::string& schema_name = "public")
{

    spdlog::info("checking table '{}'", table_name);
    std::string regclass = fmt::format("{}.{}", schema_name, table_name);
    const auto result = m_conn->execute("SELECT to_regclass( $1 );", regclass.c_str());
    return !result.is_null(0, 0);
}

void
Database::drop_table(const char* table_name) {
    spdlog::info("droping table '{}'", table_name);
    m_conn->execute(fmt::format("DROP TABLE IF EXISTS {}", table_name));
}

void
Database::create_table(const char* table_name, bool force = false)
{
    spdlog::info("creating new table '{}'", table_name);
    if (table_exists(table_name)) {
        spdlog::warn("table already exists");
        if (!force) {
            return;
        }
        drop_table(table_name);
    }

    auto columns = R"(
(
    id INTEGER PRIMARY KEY,
    url VARCHAR NOT NULL,
    length INTEGER,
    depth INTEGER,
    special_chars INTEGER,
    special_keywords INTEGER,
    hostname_ip INTEGER,
    non_std_port INTEGER,
    non_std_tld INTEGER,
    query_script INTEGER,
    query_html INTEGER,
    chars_freq INTEGER,
    timestamp TIMESTAMP WITH TIME ZONE,
    total INTEGER
)
)";

    m_conn->execute(fmt::format("CREATE TABLE {} {}", table_name, columns));
}

} // namespace phishapp
