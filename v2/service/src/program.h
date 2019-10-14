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
     * all information needed, or how the application should behave.
     */
    void check_general_opts();

    /// Gets or construct a connection string to a database
    std::string get_conn_string();
private:
    cxxopts::Options _options;

    /// General options
    bool _help;
    bool _version;

    /// Database options
    std::string _host;
    std::string _port;
    std::string _dbname;
    std::string _user;
    std::string _password;
    std::string _conn_string;

};

#endif // PHISHSVC_PROGRAM_H
