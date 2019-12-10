#include "program.h"

#include "help_functions.h"
#include "features/feature_enum.h"
#include "env.h"

#include <filesystem>
#include <cxxopts.hpp>
#include <spdlog/spdlog.h>


using namespace std::string_view_literals;
namespace fs = std::filesystem;

namespace phishscore {

program::program(int argc, char** argv)
    : _options("phishsvc", "Application for phishing defence")
{
    _options
        .positional_help("[positional args]")
        .show_positional_help();

    _options.add_options("General")
        ("h,help", "Prints help and exit", cxxopts::value<bool>(help_))
        ("v,version", "Prints version information and exit", cxxopts::value<bool>(version_))
        ("verbose", "Enable verbose mode", cxxopts::value<bool>(opts_.verbose))
    ;

    _options.add_options("Database")
        ("enable-database", "Flag whether database will be used", cxxopts::value<bool>(_enable_database))
        ("host", "Name of the host where is the database", cxxopts::value<std::string>(opts_.database.host))
        ("port", "Port number to connect to at the server host", cxxopts::value<std::string>(opts_.database.port))
        ("dbname", "The database name", cxxopts::value<std::string>(opts_.database.db_name))
        ("user", "PostgreSQL user name to connect as", cxxopts::value<std::string>(opts_.database.user))
        ("password", "Password to be used if the server demands server authentication", cxxopts::value<std::string>(opts_.database.password))
        ("connstring", "The whole connection string to use instead of separate parameters", cxxopts::value<std::string>(opts_.database.conn_string))
    ;

    _options.add_options("Table manipulation")
        ("enable-table-manipulation", "Flag wether table manipulation will be used", cxxopts::value<bool>(_enable_table_manipulation))
        ("table", "Name of the table where our operations will be performed", cxxopts::value<std::string>(opts_.parse_urls_to_table))
    ;

    _options.add_options("Training data")
        ("enable-training-data", "Flag wether we are creating a training data for ML algorithms",
            cxxopts::value<bool>(_enable_training_data))
        ("td-stdin", "Flag whether we are taking input from the commandline",
            cxxopts::value<bool>(opts_.td.stdin))
        ("td-url", "One URL to be checked",
            cxxopts::value<std::string>(opts_.td.url))
        ("td-class-value", "Sets the class label for the training data",
            cxxopts::value<double>(opts_.fvec.class_label))
        ("td-output-url", "Add to output also column with source URL",
            cxxopts::value<bool>(opts_.fvec.include_url))
        ("td-output-extra-values", "Add to output also extra values used for feature computation (<feature>_value)",
            cxxopts::value<bool>(opts_.fvec.extra_values))
        ("td-html-analysis-path", "Path to the binary with HTML analysis module",
            cxxopts::value<std::string>(opts_.html_analysis.bin_path))
    ;

    _options.add_options("Model check")
        ("enable-model-checking", "Flag whether we are checking our model",
            cxxopts::value<bool>(enable_model_checking))
        ("mc-model-checker-path", "Path to the model checker",
            cxxopts::value<std::string>(opts_.model_checker.path))
        ("mc-html-analysis-path", "Path to the binary with HTML analysis module",
            cxxopts::value<std::string>(opts_.html_analysis.bin_path))
    ;

    _options.add_options("Main application")
        ("html-analysis-host", "Hostname where is HTML analysis application",
            cxxopts::value<std::string>(opts_.html_analysis.host))
        ("html-analysis-port", "Port where is listening HTML analysis application",
            cxxopts::value<uint16_t>(opts_.html_analysis.port))
        ("model-checker-host", "Hostname where is model checking application",
            cxxopts::value<std::string>(opts_.model_checker.host))
        ("model-checker-port", "Port where is listening model checking application",
            cxxopts::value<uint16_t>(opts_.model_checker.port))
        ("stdin", "Flag whether we are taking input from the standard input",
            cxxopts::value<bool>(opts_.input.stdin))
        ("check-url", "One URL to be checked",
            cxxopts::value<std::string>(opts_.input.url))
        ("server", "Whether run as server application and listens on a port",
            cxxopts::value<uint16_t>(opts_.server))
   ;

    _options.add_options("Features")
        ("enable-features", "Enter mode with features", cxxopts::value<bool>(_enable_features))
        // URL
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
        ("feat-punycode", "Check wether host is using punycode", cxxopts::value<bool>(_feature_punycode))
        // HTML
        ("feat-input-tag", "Check wether page contains <input> tag", cxxopts::value<bool>(_feature_input_tag))
        ("feat-src-link", "Check wether links in src=<LINK> are pointing to other host", cxxopts::value<bool>(_feature_src_link))
        ("feat-form-handler", "Check wether <form> handlers has meaningful actions", cxxopts::value<bool>(_feature_form_handler))
        ("feat-invisible-iframe", "Check wether there are invisible <iframe>s", cxxopts::value<bool>(_feature_invisible_iframe))
        ("feat-rewrite-statusbar", "Check wether elements are rewriting statusbar", cxxopts::value<bool>(_feature_rewrite_statusbar))
        ("feat-disable-rightclick", "Check wether page has disabled rightlick ", cxxopts::value<bool>(_feature_disable_rightclick))
        ("feat-ahref-link", "Check wether most of the links are pointing to other sites ", cxxopts::value<bool>(_feature_ahref_link))
        ("feat-popup-window", "Check wether page has popup windows", cxxopts::value<bool>(_feature_popup_window))
        ("feat-favicon-link", "Check wether favicon is loaded from external site", cxxopts::value<bool>(_feature_favicon_link))
        ("feat-old-technologies", "Check wether page is not using new technologies", cxxopts::value<bool>(_feature_old_technologies))
        ("feat-missleading-link", "Check wether <a> elements are displaying same link as they refer to", cxxopts::value<bool>(_feature_missleading_link))
        ("feat-hostname-title", "Check wether hostname is in the title", cxxopts::value<bool>(_feature_hostname_title))
        // HOST BASED
        ("feat-redirect", "Check wether URL points to redirected site", cxxopts::value<bool>(_feature_redirect))
        ("feat-google-index", "Check wether host of URL is indexed by Google", cxxopts::value<bool>(_feature_google_index))
        ("feat-dns-a-record", "Check wether host has DNS A record", cxxopts::value<bool>(_feature_dns_a_record))
        ("feat-dnssec", "Check wether host has DNSSEC", cxxopts::value<bool>(_feature_dnssec))
        ("feat-dns-created", "Check when was DNS record created", cxxopts::value<bool>(_feature_dns_created))
        ("feat-dns-updated", "Check when was DNS record updated", cxxopts::value<bool>(_feature_dns_updated))
        ("feat-ssl-created", "Check start date of HTTPS cert", cxxopts::value<bool>(_feature_ssl_created))
        ("feat-ssl-expire", "Check expiry date of HTTPS cert", cxxopts::value<bool>(_feature_ssl_expire))
        ("feat-ssl-subject", "Check if CN from HTTPS cert matches hostname", cxxopts::value<bool>(_feature_ssl_subject))
        ("feat-hsts", "Check if website is using HSTS", cxxopts::value<bool>(_feature_hsts))
        ("feat-xss-protection", "Check if site has implemented XSS protection", cxxopts::value<bool>(_feature_xss_protection))
        ("feat-csp", "Check if website has implemented content strict policies", cxxopts::value<bool>(_feature_csp))
        ("feat-x-frame", "Check if website has implemented Clickjacking protection", cxxopts::value<bool>(_feature_x_frame))
        ("feat-x-content-type", "Check if website has implemented MIME type nosniffing", cxxopts::value<bool>(_feature_x_content_type))
        ("feat-asn", "Check wether site is in problematic ASN", cxxopts::value<bool>(_feature_asn))
        ("feat-similar-domain", "Check wether site has similar domain name as another famous site", cxxopts::value<bool>(_feature_similar_domain))
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
    if (help_) {
        fmt::print("{}\n", _options.help({"General", "Main application", "Database", "Table manipulation", "Features", "HTML feature settings", "Training data", "Model check"}));
        exit(0);
    }

    if (version_) {
        fmt::print("0.0.1\n");
        exit(0);
    }

    if (_enable_features) {
        if (opts_.verbose) fmt::print("Features:\n");
        check_url_feature_option(_feature_ip_address, feature_enum::id::ip_address, "IP address"sv);
        check_url_feature_option(_feature_url_length, feature_enum::id::url_length, "URL length"sv);
        check_url_feature_option(_feature_host_length, feature_enum::id::host_length, "host length"sv);
        check_url_feature_option(_feature_path_length, feature_enum::id::path_length, "path length"sv);
        check_url_feature_option(_feature_query_length, feature_enum::id::query_length, "query length"sv);
        check_url_feature_option(_feature_fragment_length, feature_enum::id::fragment_length, "fragment length"sv);
        check_url_feature_option(_feature_user_info, feature_enum::id::user_info, "user info"sv);
        check_url_feature_option(_feature_domain_count, feature_enum::id::domain_count, "domain count"sv);
        check_url_feature_option(_feature_https_used, feature_enum::id::https_used, "HTTPS used"sv);
        check_url_feature_option(_feature_extra_https, feature_enum::id::extra_https, "extra HTTPS"sv);
        check_url_feature_option(_feature_shortening_service, feature_enum::id::shortening_service, "shortening service"sv);
        check_url_feature_option(_feature_non_std_port, feature_enum::id::non_std_port, "non standard port"sv);
        check_url_feature_option(_feature_spec_chars_path, feature_enum::id::spec_chars_path, "special chars path"sv);
        check_url_feature_option(_feature_spec_chars_query, feature_enum::id::spec_chars_query, "special chars query"sv);
        check_url_feature_option(_feature_spec_chars_fragment, feature_enum::id::spec_chars_fragment, "special chars fragment"sv);
        check_url_feature_option(_feature_spec_chars_host, feature_enum::id::spec_chars_host, "special chars host"sv);
        check_url_feature_option(_feature_gtld, feature_enum::id::gtld, "gTLD"sv);
        check_url_feature_option(_feature_www_prefix, feature_enum::id::www_prefix, "www- prefix"sv);
        check_url_feature_option(_feature_four_numbers, feature_enum::id::four_numbers, "four consecutive numbers in host"sv);
        check_url_feature_option(_feature_spec_keywords, feature_enum::id::spec_keywords, "special keywords"sv);
        check_url_feature_option(_feature_punycode, feature_enum::id::punycode, "punycode"sv);
        // html
        check_html_feature_option(_feature_input_tag, feature_enum::id::input_tag, "<input> tag"sv);
        check_html_feature_option(_feature_src_link, feature_enum::id::src_link, "<src> links to outer world"sv);
        check_html_feature_option(_feature_form_handler, feature_enum::id::form_handler, "form handler"sv);
        check_html_feature_option(_feature_invisible_iframe, feature_enum::id::invisible_iframe, "invisible <iframe>"sv);
        check_html_feature_option(_feature_rewrite_statusbar, feature_enum::id::rewrite_statusbar, "rewrite statusbar"sv);
        check_html_feature_option(_feature_disable_rightclick, feature_enum::id::disable_rightclick, "disabled rightclick"sv);
        check_html_feature_option(_feature_ahref_link, feature_enum::id::ahref_link, "<a> links to outer world"sv);
        check_html_feature_option(_feature_popup_window, feature_enum::id::popup_window, "popup window"sv);
        check_html_feature_option(_feature_favicon_link, feature_enum::id::favicon_link, "favicon link"sv);
        check_html_feature_option(_feature_old_technologies, feature_enum::id::old_technologies, "old technologies"sv);
        check_html_feature_option(_feature_missleading_link, feature_enum::id::missleading_link, "missleading <a> link"sv);
        check_html_feature_option(_feature_hostname_title, feature_enum::id::hostname_title, "hostname in title"sv);
        // host based
        check_host_based_feature_option(_feature_redirect, feature_enum::id::redirect, "redirect"sv);
        check_host_based_feature_option(_feature_google_index, feature_enum::id::google_index, "google index"sv);
        check_host_based_feature_option(_feature_dns_a_record, feature_enum::id::dns_a_record, "DNS A record"sv);
        check_host_based_feature_option(_feature_dnssec, feature_enum::id::dnssec, "DNSSEC record"sv);
        check_host_based_feature_option(_feature_dns_created, feature_enum::id::dns_created, "DNS created"sv);
        check_host_based_feature_option(_feature_dns_updated, feature_enum::id::dns_updated, "DNS updated"sv);
        check_host_based_feature_option(_feature_ssl_created, feature_enum::id::ssl_created, "SSL start date"sv);
        check_host_based_feature_option(_feature_ssl_expire, feature_enum::id::ssl_expire, "SSL expirty date"sv);
        check_host_based_feature_option(_feature_ssl_subject, feature_enum::id::ssl_subject, "SSL subject"sv);
        check_host_based_feature_option(_feature_hsts, feature_enum::id::hsts, "HSTS"sv);
        check_host_based_feature_option(_feature_xss_protection, feature_enum::id::xss_protection, "X-XSS-Protection"sv);
        check_host_based_feature_option(_feature_csp, feature_enum::id::csp, "Content-Security-Policy"sv);
        check_host_based_feature_option(_feature_x_frame, feature_enum::id::x_frame, "X-Frame-Options"sv);
        check_host_based_feature_option(_feature_x_content_type, feature_enum::id::x_content_type, "X-Content-Type"sv);
        check_host_based_feature_option(_feature_asn, feature_enum::id::asn, "ASN"sv);
        check_host_based_feature_option(_feature_similar_domain, feature_enum::id::similar_domain, "similar domain"sv);
    }

    if (_enable_training_data) {
        if (opts_.td.url.empty() && !opts_.td.stdin) {
            fmt::print(stderr, "You have to provide one source for a data (e.g. --td-url <URL>)\n");
            exit(1);
        }

        if (!opts_.td.url.empty() && opts_.td.stdin) {
            fmt::print(stderr, "Please choose only one type of input\n");
            exit(1);
        }

        if (opts_.fvec.class_label == -1) {
            fmt::print(stderr, "You need to set classification value for the input set (--td-class-value <N>)\n");
            exit(1);
        }

        std::error_code ec;
        if (opts_.flags.html) {
            if (opts_.html_analysis.bin_path.empty()) {
                fmt::print(stderr, "You have have requested HTML features. Please set path to program (--td-html-analysis-path <path>)\n");
                exit(1);
            }
            if (!opts_.html_analysis.bin_path.empty()) {
                if (!fs::exists(opts_.html_analysis.bin_path, ec)) {
                    fmt::print(stderr, "No such file (--td-html-analysis-path <path>): {}\n", opts_.html_analysis.bin_path);
                    exit(1);
                }
            }
        }

        if (opts_.flags.all == 0) {
            fmt::print(stderr, "Feature flags are empty. Maybe you forgot to set what features you want?\n");
        }
    }

    if (enable_model_checking || !opts_.server != 0 || !opts_.input.url.empty()) {
        if (enable_model_checking && (!opts_.input.stdin && opts_.input.url.empty()) && opts_.server == 0) {
            fmt::print(stderr, "You have to provide source type for a data (e.g. --check-url <URL>)\n");
            exit(1);
        }
        std::error_code ec;
        if (opts_.html_analysis.port == 0 && opts_.html_analysis.bin_path.empty()) {
            fmt::print("--html-analysis-port = 0\n");
            opts_.html_analysis.bin_path = get_env_var("THESIS_HTML_ANALYSIS_PROG");
            if (opts_.html_analysis.bin_path.empty()) {
                fmt::print(stderr, "THESIS_HTML_ANALYSIS_PROG not set\n");
                fmt::print(stderr, "Please set path to HTML analysis program or set <HOST:PORT>\n");
                exit(1);
            }
        }
        if (opts_.html_analysis.port == 0 && !fs::exists(opts_.html_analysis.bin_path, ec)) {
            fmt::print(stderr, "ERROR (HTML analysis module). No such file: {}\n", opts_.html_analysis.bin_path);
            exit(1);
        }
        if (opts_.model_checker.port == 0 && opts_.model_checker.path.empty()) {
            fmt::print("--model-checker-port = 0\n");
            opts_.model_checker.path = get_env_var("THESIS_MODEL_CHECKER_PROG");
            if (opts_.model_checker.path.empty()) {
                fmt::print(stderr, "THESIS_MODEL_CHECKER_PROG not set\n");
                fmt::print(stderr, "Please set path to model checker program or set <HOST:PORT>\n");
                exit(1);
            }
        }
        if (opts_.model_checker.port == 0 && !fs::exists(opts_.model_checker.path, ec)) {
            fmt::print(stderr, "ERROR (Model checker module): No such file: {}\n", opts_.model_checker.path);
            exit(1);
        }

        if (opts_.html_analysis.port != 0) {
            fmt::print("Resolving HTML analysis host: {}\n", opts_.html_analysis.host);
            auto cmd = fmt::format("getent ahostsv4 {} | awk '{{ print $1 }}'", opts_.html_analysis.host);
            auto host = help_functions::get_output_from_program(cmd);
            if (!host.empty()) {
                fmt::print("Resolved: {}\n", host.front());
                opts_.html_analysis.host = host.front();
            } else {
                fmt::print(stderr, "Error. Can't resolve host.\n");
            }
        }

        if (opts_.model_checker.port != 0) {
            fmt::print("Resolving model checker host: {}\n", opts_.model_checker.host);
            auto cmd = fmt::format("getent ahostsv4 {} | awk '{{ print $1 }}'", opts_.model_checker.host);
            auto host = help_functions::get_output_from_program(cmd);
            if (!host.empty()) {
                fmt::print("Resolved: {}\n", host.front());
                opts_.model_checker.host = host.front();
            } else {
                fmt::print(stderr, "Error. Can't resolve host.\n");
            }
        }
        if (opts_.verbose) {
            fmt::print("HTML analysis host: {}\n", opts_.html_analysis.host);
            fmt::print("HTML analysis port: {}\n", opts_.html_analysis.port);
            fmt::print("Model checker host: {}\n", opts_.model_checker.host);
            fmt::print("Model checker port: {}\n", opts_.model_checker.port);
        }
        // we need the following columns for the model prediction
        // --feat-asn --feat-similar-domain --feat-dnssec --feat-gtld --feat-src-link --feat-dns-updated
        // --feat-host-length --feat-dns-created --feat-ahref-link --feat-favicon-link --feat-spec-keywords
        // --feat-path-length --feat-url-length --feat-spec-chars-query --feat-spec-chars-path
        check_url_feature_option(true, feature_enum::id::host_length, "host length"sv);
        check_url_feature_option(true, feature_enum::id::spec_keywords, "special keywords"sv);
        check_url_feature_option(true, feature_enum::id::path_length, "path length"sv);
        check_url_feature_option(true, feature_enum::id::url_length, "url length"sv);
        check_url_feature_option(true, feature_enum::id::spec_chars_query, "special chars query"sv);
        check_url_feature_option(true, feature_enum::id::spec_chars_path, "special chars path"sv);
        check_url_feature_option(true, feature_enum::gtld, "global TLD"sv);
        check_html_feature_option(true, feature_enum::ahref_link, "<a href=\"\"> link"sv);
        check_html_feature_option(true, feature_enum::favicon_link, "favicon link"sv);
        check_html_feature_option(true, feature_enum::src_link, "src link"sv);
        check_host_based_feature_option(true, feature_enum::asn, "ASN"sv);
        check_host_based_feature_option(true, feature_enum::similar_domain, "similar domain"sv);
        check_host_based_feature_option(true, feature_enum::dnssec, "dnssecn"sv);
        check_host_based_feature_option(true, feature_enum::dns_updated, "dns updated"sv);
        check_host_based_feature_option(true, feature_enum::dns_created, "dns created"sv);
    }

    // check database options
    if (_enable_database) {
        if (opts_.database.conn_string.empty() &&
            opts_.database.host.empty() && opts_.database.port.empty() &&
            opts_.database.db_name.empty() && opts_.database.user.empty() &&
            opts_.database.password.empty())
        {
            fmt::print(stderr, "You have to provide either full connection string to a database\n");
            fmt::print(stderr, "or enter parameters for connecting. Check PostgreSQL connection\n");
            fmt::print(stderr, "for a reference or consult --help of this application.\n");
            exit(1);
        }

        // construct a connection string from parameters
        if (opts_.database.conn_string.empty()) {
            opts_.database.conn_string = fmt::format("host = '{}' port = '{}' dbname = '{}' user = '{}' password = '{}'",
                opts_.database.host, opts_.database.port, opts_.database.db_name, opts_.database.user, opts_.database.password);
        }
    }

    // checking options for a one table manipulation
    if (!opts_.parse_urls_to_table.empty()) {
        _table_manipulation = true;
    }
}

void program::check_feature_option(bool feature_on, uint64_t feature_id, std::string_view feature_name)
{
    if (opts_.verbose) fmt::print("--> {} - {}\n", feature_name, feature_on ? "ON" : "OFF");
    if (feature_on) {
        opts_.flags.all |= feature_id;
    }
}

void program::check_url_feature_option(bool feature_on, uint64_t feature_id, std::string_view feature_name)
{
    check_feature_option(feature_on, feature_id, feature_name);
    if (feature_on) {
        opts_.flags.url |= feature_id;
    }
}

void program::check_html_feature_option(bool feature_on, uint64_t feature_id, std::string_view feature_name)
{
    check_feature_option(feature_on, feature_id, feature_name);
    if (feature_on) {
        opts_.flags.html |= feature_id;
    }
}

void program::check_host_based_feature_option(bool feature_on, uint64_t feature_id, std::string_view feature_name)
{
    check_feature_option(feature_on, feature_id, feature_name);
    if (feature_on) {
        opts_.flags.host_based |= feature_id;
    }
}

bool program::table_manipulation()
{
    return _table_manipulation;
}

const char* program::on_off(bool feature)
{
    return feature ? "ON" : "OFF";
}

bool program::create_training_data()
{
    return _enable_training_data;
}

} // namespace phishscore
