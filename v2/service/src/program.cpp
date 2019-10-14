#include "program.h"

#include <cxxopts.hpp>
#include <spdlog/spdlog.h>

program::program(int argc, char** argv)
    : _options("phishsvc", "Application for phishing defence")
    , _help(false)
    , _version(false)
{
    _options
        .positional_help("[positional args]")
        .show_positional_help();

    _options.add_options("General")
        ("h,help", "Prints help and exit", cxxopts::value<bool>(_help))
        ("v,version", "Prints version information and exit", cxxopts::value<bool>(_version))
    ;

    _options.add_options("Database")
        ("host", "Name of the host where is the database", cxxopts::value<std::string>(_host))
        ("port", "Port number to connect to at the server host", cxxopts::value<std::string>(_port))
        ("dbname", "The database name", cxxopts::value<std::string>(_dbname))
        ("user", "PostgreSQL user name to connect as", cxxopts::value<std::string>(_user))
        ("password", "Password to be used if the server demands server authentication", cxxopts::value<std::string>(_password))
        ("connstring", "The whole connection string to use instead of separate parameters", cxxopts::value<std::string>(_conn_string))
    ;

    auto _result = _options.parse(argc, argv);
}

void program::check_general_opts()
{
    if (_help) {
        fmt::print("{}\n", _options.help({"General", "Database"}));
        exit(0);
    }

    if (_version) {
        fmt::print("0.0.1\n");
        exit(0);
    }

    // check database options
    if (_conn_string.empty() && _host.empty() && _port.empty() && _dbname.empty() &&
        _user.empty() && _password.empty())
    {
        fmt::print(stderr, "You have to provide either full connection string to a database\n");
        fmt::print(stderr, "or enter parameters for connecting. Check PostgreSQL connection\n");
        fmt::print(stderr, "for a reference or consult --help of this application.\n");
        exit(1);
    }

    // construct a connection string from parameters
    if (_conn_string.empty()) {
        _conn_string = fmt::format("host = '{}' port = '{}' dbname = '{}' user = '{}' password = '{}'",
            _host, _port, _dbname, _user, _password);
    }
}

std::string program::get_conn_string()
{
    return _conn_string;
}
