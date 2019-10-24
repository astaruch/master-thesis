#pragma once

#ifndef PHISHSVC_PROGRAM_H
#define PHISHSVC_PROGRAM_H

#include <cxxopts.hpp>

class program {
public:
    program(int argc, char** argv);

    /// Performs general operations from user input
    /**
     * This method will perform general checking, whether user provided
     * all information needed, or how the application should behave, and
     * prepare for the next steps.
     */
    void check_options();

    /// Gets or construct a connection string to a database
    std::string get_conn_string();

    /// Check whether we are going to manipulating with a table
    bool table_manipulation();

    bool parse_urls();
    std::string table_name();
private:
    cxxopts::Options _options;

    /// General options
    bool _help;
    bool _version;

    /// Disambiguation options
    bool _enable_features;
    bool _enable_database;
    bool _enable_table_manipulation;

    std::string _host;
    std::string _port;
    std::string _dbname;
    std::string _user;
    std::string _password;
    std::string _conn_string;

    /// Table manipulation options
    bool _table_manipulation;
    std::string _table;
    bool _parse_urls;

    /// Features
    bool _feature_ip_address;

};

#endif // PHISHSVC_PROGRAM_H
