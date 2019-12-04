#pragma once

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


    explicit database();
    explicit database(const std::string& conn_string);

    /// Insert new columns for URL parts into given table
    void add_url_columns(const std::string& table_name);

    /// Gets a set with column names for a particular table
    std::set<std::string> get_column_names(const std::string& table_name);

    /// Prepare a statement with name "update_url_parts" for database
    void prepare_update_url_parts(const std::string& table_name, pqxx::work& txn);

    /// Executes a statement with name "update_url_parts"
    /**
     * We need to hack our way around and pass pointers, so we can check
     * if value is empty and pass NULL value to SQL command.
     */
    pqxx::result execute_update_url_parts(pqxx::work& txn, int id,
        const std::string* scheme,
        const std::string* user_info,
        const std::string* host,
        unsigned int* port,
        const std::string* path,
        const std::string* query,
        const std::string* fragment);

    /// Executes a statement with name "update_url_parts"
    /**
     * TODO: optimize to not create a copy of strings
     * This method creates a temporary copy so we can pass a pointer to execute
     * method. This is not optimal, but I don't know the other way how, how to
     * pass either value or NULL to sql.
     */
    pqxx::result execute_update_url_parts(pqxx::work& txn, int id,
        const Poco::URI& url);

    /// Gets a records with ID & URL
    std::vector<database::db_record> get_all_records(const std::string& table_name);

    /// Fill the database with parsed URLs
    bool fill_db_with_url_parts(const std::string& table_name,
        std::vector<database::db_record>& records);

    void test_connection();

    /// Parse URLs from the given table into new columns
    void process_table_and_parse_urls(const std::string& table_name);

    void debug_record(const db_record& record);

private:
    pqxx::connection _conn;
    // pqxx::work _txn;

    static std::string create_conn_string();
};

#endif // PHISHSVC_DATABASE_H
