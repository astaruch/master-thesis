#include "program.h"

#include <cxxopts.hpp>
#include <spdlog/spdlog.h>

program::program(int argc, char** argv)
    : _options("phishsvc", "Application for phishing defence")
    , _help(false)
    , _version(false)
    , _feature_flags(0)
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
        ("feat-ip-address", "Check wether hostname is IP address", cxxopts::value<bool>(_feature_ip_address))
        ("feat-url-length", "Check logness of the URL", cxxopts::value<bool>(_feature_url_length))
        ("feat-host-length", "Check logness of the hostname", cxxopts::value<bool>(_feature_host_length))
        ("feat-path-length", "Check logness of the path", cxxopts::value<bool>(_feature_path_length))
        ("feat-query-length", "Check logness of the query", cxxopts::value<bool>(_feature_query_length))
        ("feat-fragment-length", "Check logness of the fragment", cxxopts::value<bool>(_feature_fragment_length))
        ("feat-user-info", "Check wether URL contains user info", cxxopts::value<bool>(_feature_user_info))
    ;

    _options.add_options("Training data")
        ("enable-training-data", "Flag wether we are creating a training data for ML algorithms",
            cxxopts::value<bool>(_enable_training_data))
        ("td-url", "Enter one escaped URL as parameter",
            cxxopts::value<std::string>(_training_data_url))
        ("td-stdin", "Flag that we are using standard input as source",
            cxxopts::value<bool>(_training_data_stdin))
        ("td-input-file", "Path containing escaped URLs delimitaded by a new line",
            cxxopts::value<std::string>(_training_data_input_file))
        ("td-output-name", "Desired name of the output data.",
            cxxopts::value<std::string>(_training_data_output_name)
                ->default_value("training-data.csv")
                ->implicit_value("training-data.csv"))
        ("td-class-value", "Sets the classification value for the training data",
            cxxopts::value<double>(_training_data_class_value))
    ;

    try {
        auto _result = _options.parse(argc, argv);
        _missing_training_data_class_value = _result.count("td-class-value") == 0;
    } catch (const cxxopts::option_not_exists_exception& ex) {
        fmt::print(stderr, "{}\n", ex.what());
        exit(1);
    }
}

void program::check_options()
{
    if (_help) {
        fmt::print("{}\n", _options.help({"General", "Database", "Table manipulation", "Features", "Training data"}));
        exit(0);
    }

    if (_version) {
        fmt::print("0.0.1\n");
        exit(0);
    }

    if (_enable_features) {
        fmt::print("Features:\n");
        fmt::print("-- IP address - {}\n", on_off(_feature_ip_address));
        if (_feature_ip_address) {
            _feature_flags |= feature_name::ip_address;
        }
        fmt::print("-- URL length - {}\n", on_off(_feature_url_length));
        if (_feature_url_length) {
            _feature_flags |= feature_name::url_length;
        }
        fmt::print("-- host length - {}\n", on_off(_feature_host_length));
        if (_feature_host_length) {
            _feature_flags |= feature_name::host_length;
        }
        fmt::print("-- path length - {}\n", on_off(_feature_path_length));
        if (_feature_path_length) {
            _feature_flags |= feature_name::path_length;
        }
        fmt::print("-- query length - {}\n", on_off(_feature_query_length));
        if (_feature_query_length) {
            _feature_flags |= feature_name::query_length;
        }
        fmt::print("-- fragment length - {}\n", on_off(_feature_fragment_length));
        if (_feature_fragment_length) {
            _feature_flags |= feature_name::fragment_length;
        }
        fmt::print("-- user info - {}\n", on_off(_feature_user_info));
        if (_feature_user_info) {
            _feature_flags |= feature_name::user_info;
        }
    }

    if (_enable_training_data) {
        if (_training_data_url.empty() && !_training_data_stdin && _training_data_input_file.empty()) {
            fmt::print(stderr, "You have to provide one source for data\n");
            exit(1);
        }

        if (_training_data_input_file.size() > 0 || _training_data_stdin) {
            fmt::print(stderr, "Not implemented yet\n");
            exit(1);
        }

        if (_missing_training_data_class_value) {
            fmt::print(stderr, "You need to set classification value for the input set\n");
            exit(1);
        }

        if (_feature_flags == 0) {
            fmt::print(stderr, "Feature flags are empty. Maybe you forgot to set what features you want?\n");
        }
    }

    // check database options
    if (_enable_database) {
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

const char* program::on_off(bool feature)
{
    return feature ? "ON" : "OFF";
}

uint64_t program::feature_flags()
{
    return _feature_flags;
}

std::string program::training_data_url()
{
    return _training_data_url;
}

std::string program::training_data_output_name()
{
    return _training_data_output_name;
}

bool program::create_training_data()
{
    return _enable_training_data;
}

double program::training_data_class_value()
{
    return _training_data_class_value;
}
