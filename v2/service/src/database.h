#ifndef PHISHSVC_DATABASE_H
#define PHISHSVC_DATABASE_H

#include <set>
#include <string>
#include <vector>

#include <pqxx/connection>
#include <pqxx/transaction>

class database {
public:
    /// Default constructor. Creates a connection to database
    /**
     * Initializes private member _conn and _txn for later use.
     */
    explicit database(const std::string& conn_string);

    /// Insert new columns for URL parts into given table
    void add_url_columns(const std::string& table_name);

    /// Gets a set with column names for a particular table
    std::set<std::string> get_column_names(const std::string& table_name);


    /// Test whether connection is valid
    void test_connection();

private:
    pqxx::connection _conn;
    pqxx::work _txn;
};

#endif // PHISHSVC_DATABASE_H
