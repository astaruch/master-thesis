#ifndef PHISHSVC_DATABASE_H
#define PHISHSVC_DATABASE_H

#include <algorithm>
#include <set>
#include <string>
#include <vector>

#include <pqxx/connection>
#include <pqxx/transaction>
#include <Poco/URI.h>
#include <Poco/Exception.h>


class database {
public:
    /// Wrapper around database record
    struct db_record {
        int id; /// ID of record obtained from database
        std::string full_url; /// Raw URL of a record obtained from database
        Poco::URI url_obj; /// Parsed URL object obtained from library
        bool ok = false; /// Whether parsing of URL was OK
    };

    /// Default constructor. Creates a connection to database
    /**
     * Initializes private member _conn and _txn for later use.
     */
    explicit database(const std::string& conn_string);

    /// Insert new columns for URL parts into given table
    void add_url_columns(const std::string& table_name);

    /// Gets a set with column names for a particular table
    std::set<std::string> get_column_names(const std::string& table_name);

    /// Prepare a statement with name "update_url_parts" for database
    void prepare_update_url_parts();

    /// Executes a statement with name "update_url_parts"
    pqxx::result execute_update_url_parts(const std::string& table_name,
        int id, const std::string& scheme,
        const std::string& user_info,
        const std::string& host,
        unsigned int port,
        const std::string& path,
        const std::string& query,
        const std::string& fragment);

    /// Executes a statement with name "update_url_parts"
    pqxx::result execute_update_url_parts(const std::string& tabale_name,
        int id,
        const Poco::URI& url);

    /// Gets a records with ID & URL
    std::vector<database::db_record> get_all_records(const std::string& table_name);


    /// Test whether connection is valid
    void test_connection();

private:
    pqxx::connection _conn;
    pqxx::work _txn;
};

#endif // PHISHSVC_DATABASE_H
