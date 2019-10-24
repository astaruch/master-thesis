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
        ("enable-database", "Flag whether database will be used", cxxopts::value<bool>(_enable_database))
        ("host", "Name of the host where is the database", cxxopts::value<std::string>(_host))
        ("port", "Port number to connect to at the server host", cxxopts::value<std::string>(_port))
        ("dbname", "The database name", cxxopts::value<std::string>(_dbname))
        ("user", "PostgreSQL user name to connect as", cxxopts::value<std::string>(_user))
        ("password", "Password to be used if the server demands server authentication", cxxopts::value<std::string>(_password))
        ("connstring", "The whole connection string to use instead of separate parameters", cxxopts::value<std::string>(_conn_string))
    ;

    _options.add_options("Table manipulation")
        ("enable-table-manipulation", "Flag wether table manipulation will be used", cxxopts::value<bool>(_enable_table_manipulation))
        ("table", "Name of the table where our operations will be performed", cxxopts::value<std::string>(_table))
        ("parse-urls", "Parse URLs from the given table into new columns", cxxopts::value<bool>(_parse_urls))
    ;

    _options.add_options("Features")
        ("enable-features", "Enter mode with features", cxxopts::value<bool>(_enable_features))
        ("ip-address", "Check whether hostname is IP address", cxxopts::value<bool>(_feature_ip_address))
    ;

    try {
        auto _result = _options.parse(argc, argv);
    } catch (const cxxopts::option_not_exists_exception& ex) {
        fmt::print(stderr, "{}\n", ex.what());
        exit(1);
    }
}

void program::check_options()
{
    if (_help) {
        fmt::print("{}\n", _options.help({"General", "Database", "Table manipulation", "Features"}));
        exit(0);
    }

    if (_version) {
        fmt::print("0.0.1\n");
        exit(0);
    }

    if (_enable_features) {
        fmt::print("Checking features");
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

    // checking options for a one table manipulation
    if (!_table.empty() || _parse_urls) {
        _table_manipulation = true;
    }
}

std::string program::get_conn_string()
{
    return _conn_string;
}

bool program::table_manipulation()
{
    return _table_manipulation;
}

bool program::parse_urls()
{
    return _parse_urls;
}

std::string program::table_name()
{
    return _table;
}
