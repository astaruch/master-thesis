#include "program.h"

#include <cxxopts.hpp>
#include <spdlog/spdlog.h>

using namespace std::string_view_literals;

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
        ("feat-domain-count", "Check number of domains", cxxopts::value<bool>(_feature_domain_count))
        ("feat-https-used", "Check wether site is using HTTPS", cxxopts::value<bool>(_feature_https_used))
        ("feat-extra-https", "Check wether URL contains extra HTTPS token", cxxopts::value<bool>(_feature_extra_https))
        ("feat-shortening-service", "Check wether URL uses shortening service", cxxopts::value<bool>(_feature_shortening_service))
        ("feat-non-std-port", "Check wether used port is standard", cxxopts::value<bool>(_feature_non_std_port))
        ("feat-spec-chars-path", "Check occurences of special characters in a path", cxxopts::value<bool>(_feature_spec_chars_path))
        ("feat-spec-chars-query", "Check occurences of special characters in a query", cxxopts::value<bool>(_feature_spec_chars_query))
        ("feat-spec-chars-fragment", "Check occurences of special characters in a fragment", cxxopts::value<bool>(_feature_spec_chars_fragment))
        ("feat-spec-chars-host", "Check occurences of dash and underscore a host", cxxopts::value<bool>(_feature_spec_chars_host))
        ("feat-gtld", "Check wether TLD is global or othery", cxxopts::value<bool>(_feature_gtld))
        ("feat-www-prefix", "Check wether hostname has extra 'www-' prefix", cxxopts::value<bool>(_feature_www_prefix))
        ("feat-four-numbers", "Check wether hostname has 4 consecutive numbers", cxxopts::value<bool>(_feature_four_numbers))
        ("feat-spec-keywords", "Check wether URL contains special keywords", cxxopts::value<bool>(_feature_spec_keywords))
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

void program::check_feature_option(bool feature_on, uint64_t feature_id, std::string_view feature_name)
{
    fmt::print("-- {} - {}\n", feature_name, feature_on ? "ON" : "OFF");
    if (feature_on) {
        _feature_flags |= feature_id;
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
        check_feature_option(_feature_ip_address, feature_enum::id::ip_address, "IP address"sv);
        check_feature_option(_feature_url_length, feature_enum::id::url_length, "URL length"sv);
        check_feature_option(_feature_host_length, feature_enum::id::host_length, "host length"sv);
        check_feature_option(_feature_path_length, feature_enum::id::path_length, "path length"sv);
        check_feature_option(_feature_query_length, feature_enum::id::query_length, "query length"sv);
        check_feature_option(_feature_fragment_length, feature_enum::id::fragment_length, "fragment length"sv);
        check_feature_option(_feature_user_info, feature_enum::id::user_info, "user info"sv);
        check_feature_option(_feature_domain_count, feature_enum::id::domain_count, "domain count"sv);
        check_feature_option(_feature_https_used, feature_enum::id::https_used, "HTTPS used"sv);
        check_feature_option(_feature_extra_https, feature_enum::id::extra_https, "extra HTTPS"sv);
        check_feature_option(_feature_shortening_service, feature_enum::id::shortening_service, "shortening service"sv);
        check_feature_option(_feature_non_std_port, feature_enum::id::non_std_port, "non standard port"sv);
        check_feature_option(_feature_spec_chars_path, feature_enum::id::spec_chars_path, "special chars path"sv);
        check_feature_option(_feature_spec_chars_query, feature_enum::id::spec_chars_query, "special chars query"sv);
        check_feature_option(_feature_spec_chars_fragment, feature_enum::id::spec_chars_fragment, "special chars fragment"sv);
        check_feature_option(_feature_spec_chars_host, feature_enum::id::spec_chars_host, "special chars host"sv);
        check_feature_option(_feature_gtld, feature_enum::id::gtld, "gTLD"sv);
        check_feature_option(_feature_www_prefix, feature_enum::id::www_prefix, "www- prefix"sv);
        check_feature_option(_feature_four_numbers, feature_enum::id::four_numbers, "four consecutive numbers in host"sv);
        check_feature_option(_feature_spec_keywords, feature_enum::id::spec_keywords, "special keywords"sv);
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
