#include "host_based_features.h"

#include "../help_functions.h"

#include <algorithm>
#include <regex>
#include <string_view>
#include <thread>
#include <future>

#include <edlib.h>
#include <fmt/format.h>
#include <Poco/Exception.h>

host_based_features_t::host_based_features_t(const std::string_view url)
try
   : _url(url)
   , _parsed(Poco::URI(url.begin()))
{
}
catch (const Poco::SyntaxException& ex)
{
}

host_based_features_t::host_based_features_t(const std::string_view url,
                                             const Poco::URI& parsed,
                                             const uint64_t flags)
    : _url(url)
    , _flags(flags)
    , _parsed(parsed)
{
    std::vector<std::thread> threads;
    std::thread dig_thread; // need to be first resolved because ASN is depending on it
    if (_flags & (feature_enum::dns_a_record | feature_enum::dnssec | feature_enum::asn)) {
        dig_thread = std::thread(&host_based_features_t::fill_dig_response, this);
    }
    std::thread sld_thread; // need to have separate thread because similar domain is dependant on sld
    if (_flags & (feature_enum::similar_domain)) {
        // fmt::print("Spawning SLD response thread\n");
        sld_thread = std::thread(&host_based_features_t::fill_sld, this);
    }
    if (_flags & (feature_enum::ssl_created | feature_enum::ssl_expire | feature_enum::ssl_subject)) {
        // fmt::print("Spawning SSL response thread\n");
        threads.push_back(std::thread(&host_based_features_t::fill_ssl_response, this));
    }
    if (_flags & (feature_enum::hsts | feature_enum::xss_protection |
                  feature_enum::csp | feature_enum::x_content_type))
    {
        // fmt::print("Spawning HTTP response\n");
        threads.push_back(std::thread(&host_based_features_t::fill_http_resp_headers, this));
    }
    if (_flags & (feature_enum::dns_created | feature_enum::dns_updated)) {
        // fmt::print("Spawning WHOIS response thread\n");
        threads.push_back(std::thread(&host_based_features_t::fill_whois_response, this));
    }
    if (_flags & (feature_enum::google_index)) {
        threads.push_back(std::thread(&host_based_features_t::fill_google_index, this));
    }
    if (dig_thread.joinable()) {
        dig_thread.join();
    }
    if (_flags & (feature_enum::asn)) {
        // we need to have dig response before we can operate with SLD due to IP address
        // fmt::print("Spawning ASN response thread\n");
        threads.push_back(std::thread(&host_based_features_t::fill_asn, this));
    }
    if (sld_thread.joinable()) {
        sld_thread.join();
    }
    if (_flags & (feature_enum::similar_domain)) {
        // similard omain name is dependant on sld, so we must first finish work in sld thread
        threads.push_back(std::thread(&host_based_features_t::fill_similar_domain_name, this));
    }
    for (size_t i = 0; i < threads.size(); i++) {
        // fmt::print("Joinin {} thread\n", i);
        threads[i].join();
    }
}

std::unordered_map<feature_enum::id, double> host_based_features_t::compute_values_map() const
{
    std::unordered_map<feature_enum::id, double> values;
    for (const auto& feature: feature_enum::host_based) {
        if (_flags & feature) {
            values[feature] = compute_value(feature);
        }
    }
    return values;
}

std::vector<double> host_based_features_t::compute_values_vec() const
{
    std::vector<double> values;
    for (const auto feature: feature_enum::host_based) {
        if (_flags & feature) {
            values.push_back(compute_value(feature));
        }
    }
    return values;
}

double host_based_features_t::compute_value_redirect(std::string_view str)
{
    http_resp_headers_ = help_functions::str2vec(str);
    return compute_value_redirect();
}

double host_based_features_t::compute_value_redirect() const
{
    // we don't have a response, thus we can't determine if the code is 301
    if (http_resp_headers_.empty()) {
        return 0;
    }
    auto status_line = http_resp_headers_[0];
    if (status_line.find("301") == std::string::npos) {
        return 0;
    }
    return 1;
}

void host_based_features_t::fill_google_index()
{
    google_indexed_ = get_is_google_indexed();
}

bool host_based_features_t::get_is_google_indexed() const
{
    auto cmd = fmt::format("curl --max-time 2 -s 'https://www.google.com/search?hl=en&q=site%3A{}' -H '{}' --compressed | grep 'did not match any documents'",
                           _parsed.getHost(), _user_agent);
    // we are looking for 'did not match any documents' => if there are not documents (empty results), site is not indexed
    auto output = help_functions::get_output_from_program(cmd);
    return output.empty(); // => it is empty, because grep didn't find the term, so we had some results -> page is indexed
}

double host_based_features_t::compute_value_google_indexed(std::string_view str)
{
    google_indexed_ = str.empty();
    return compute_value_google_indexed();
}

double host_based_features_t::compute_value_google_indexed() const
{
    return google_indexed_ ? 0 : 1;
}

std::vector<std::string> host_based_features_t::get_dig_response() const
{
    auto cmd = fmt::format("dig +timeout={} +dnssec +short {}", timeout_, _parsed.getHost());
    return help_functions::get_output_from_program(cmd);
}

void host_based_features_t::fill_dig_response()
{
    if (dig_response_.empty()) {
        dig_response_ = get_dig_response();
    }
}

double host_based_features_t::compute_value_dns_a_record() const
{
    return dig_response_.empty() ? 1 : 0;
}

double host_based_features_t::compute_value_dns_a_record(std::string_view str)
{
    dig_response_ = help_functions::str2vec(str);
    return compute_value_dns_a_record();
}

double host_based_features_t::compute_value_dnssec() const
{
    return dig_response_.size() > 1 ? 0 : 1;
}

double host_based_features_t::compute_value_dnssec(std::string_view str)
{
    dig_response_ = help_functions::str2vec(str);
    return compute_value_dnssec();
}

bool host_based_features_t::check_value_in_output(
    const std::vector<std::string>& output,
    const std::regex& reg) const
{
    for (const auto& line: output) {
        if(std::regex_search(line, reg)) {
            return true;
        }
    }
    return false;
}

std::string host_based_features_t::extract_value_from_output(
    const std::vector<std::string>& output,
    const std::regex& reg) const
{
    std::smatch match;
    std::string value;
    for (const auto& line: output) {
        if(std::regex_search(line, match, reg)) {
            value = match[2].str(); //TODO: change to last/or take as argument
            break;
        }
    }
    return value;
}

void host_based_features_t::fill_whois_response()
{
    if (whois_response_.empty()) {
        whois_response_ = get_whois_response();
    }
}


std::vector<std::string> host_based_features_t::get_whois_response() const
{
    auto cmd = fmt::format("timeout {} whois {}", timeout_, _parsed.getHost());
    return help_functions::get_output_from_program(cmd);
}

std::string host_based_features_t::extract_dns_date(const std::regex& reg) const
{
    auto value = extract_value_from_output(whois_response_, reg);
    if (value.find('.') != std::string::npos) {
        value = fmt::format("{}-{}-{}", value.substr(6,4), value.substr(3,2), value.substr(0,2));
    }
    // remove punctuation to have clear number
    value.erase(remove_if(value.begin(), value.end(), ispunct), value.end());
    if (value.empty()) {
        return "";
    }
    // add dashes to look it like a number
    value = fmt::format("{}-{}-{}", value.substr(0, 4), value.substr(4, 2), value.substr(6, 2));
    return value;
}

std::string host_based_features_t::get_dns_created(std::string_view str)
{
    whois_response_ = help_functions::str2vec(str);
    return get_dns_created();
}

std::string host_based_features_t::get_dns_updated(std::string_view str)
{
    whois_response_ = help_functions::str2vec(str);
    return get_dns_updated();
}

std::string host_based_features_t::get_dns_created() const
{
    auto reg_created = R"(^\W*(creat|registration|registered)(?:.*)(\d{8}|\d{4}\/\d{2}\/\d{2}|\d{4}-\d{2}-\d{2}|\d{2}\.\d{2}\.\d{4}))";
    const std::regex reg(reg_created, std::regex::icase);
    return extract_dns_date(reg);
}

std::string host_based_features_t::get_dns_updated() const
{
    auto reg_updated = R"(^\W*(chang|updat)(?:.*)(\d{8}|\d{4}\/\d{2}\/\d{2}|\d{4}-\d{2}-\d{2}|\d{2}\.\d{2}\.\d{4}))";
    const std::regex reg(reg_updated, std::regex::icase);
    return extract_dns_date(reg);
}

double host_based_features_t::compute_value_dns_created() const
{
    auto created = get_dns_created();
    if (created.empty()) {
        // could not find basic DNS record in traditional format
        return 1.;
    }
    return help_functions::normalize_date_string(created);
}

double host_based_features_t::compute_value_dns_updated() const
{
    auto updated = get_dns_updated();
    if (updated.empty()) {
        // could not find basic DNS record in traditional format
        return 1.;
    }
    return help_functions::normalize_date_string(updated);
}

std::vector<std::string> host_based_features_t::get_ssl_response() const
{
    auto cmd = fmt::format("echo | timeout {} openssl s_client -connect {}:{} 2>/dev/null | openssl x509 -noout -subject -dates 2>/dev/null ",
                           timeout_, _parsed.getHost(), _parsed.getPort());
    return help_functions::get_output_from_program(cmd);
}

void host_based_features_t::fill_ssl_response()
{
    if (ssl_response_.empty()) {
        ssl_response_ = get_ssl_response();
    }
}

std::string host_based_features_t::get_ssl_created() const
{
    const std::regex reg("(notBefore)=(.*)", std::regex::icase);
    auto value = extract_value_from_output(ssl_response_, reg);
    // fmt::print(value);
    return value;
}

double host_based_features_t::compute_value_ssl_created(std::string_view str)
{
    ssl_response_ = help_functions::str2vec(str);
    return compute_value_ssl_created();
}

double host_based_features_t::compute_value_ssl_created() const
{
    auto date = get_ssl_created();
    return 0;
}

std::string host_based_features_t::get_ssl_expire() const
{
    const std::regex reg("(notAfter)=(.*)", std::regex::icase);
    auto value = extract_value_from_output(ssl_response_, reg);
    // fmt::print(value);
    return value;
}

double host_based_features_t::compute_value_ssl_expire(std::string_view str)
{
    ssl_response_ = help_functions::str2vec(str);
    return compute_value_ssl_expire();
}

double host_based_features_t::compute_value_ssl_expire() const
{
    auto date = get_ssl_expire();
    return 0;
}

std::string host_based_features_t::get_ssl_subject() const
{
    const std::regex reg("subject=.*(CN = )(.*$| )", std::regex::icase);
    auto value = extract_value_from_output(ssl_response_, reg);
    return value;
}

double host_based_features_t::compute_value_ssl_subject(std::string_view str)
{
    ssl_response_ = help_functions::str2vec(str);
    return compute_value_ssl_subject();
}

double host_based_features_t::compute_value_ssl_subject() const
{
    return get_ssl_subject() == _parsed.getHost() ? 0 : 1;
}

void host_based_features_t::fill_http_resp_headers()
{
    if (http_resp_headers_.empty()) {
        http_resp_headers_ = get_http_resp_headers();
    }
}

std::vector<std::string> host_based_features_t::get_http_resp_headers() const
{
    auto cmd = fmt::format("curl --max-time 1 -s -I -X GET '{}'", _url);
    return help_functions::get_output_from_program(cmd);
}

double host_based_features_t::compute_value_hsts(std::string_view str)
{
    http_resp_headers_ = help_functions::str2vec(str);
    return compute_value_hsts();
}

double host_based_features_t::compute_value_hsts() const
{
    const std::regex reg("strict-transport-security", std::regex::icase);
    return check_value_in_output(http_resp_headers_, reg) ? 0 : 1;
}

double host_based_features_t::compute_value_xss_protection(std::string_view str)
{
    http_resp_headers_ = help_functions::str2vec(str);
    return compute_value_xss_protection();
}

double host_based_features_t::compute_value_xss_protection() const
{
    const std::regex reg("x-xss-protection:.*1", std::regex::icase);
    return check_value_in_output(http_resp_headers_, reg) ? 0 : 1;
}

double host_based_features_t::compute_value_csp(std::string_view str)
{
    http_resp_headers_ = help_functions::str2vec(str);
    return compute_value_csp();
}

double host_based_features_t::compute_value_csp() const
{
    const std::regex reg("content-security-policy", std::regex::icase);
    return check_value_in_output(http_resp_headers_, reg) ? 0 : 1;
}

double host_based_features_t::compute_value_x_frame(std::string_view str)
{
    http_resp_headers_ = help_functions::str2vec(str);
    return compute_value_x_frame();
}

double host_based_features_t::compute_value_x_frame() const
{
    const std::regex reg("x-frame-options:.*(sameorigin|deny)", std::regex::icase);
    return check_value_in_output(http_resp_headers_, reg) ? 0 : 1;
}

double host_based_features_t::compute_value_x_content_type(std::string_view str)
{
    http_resp_headers_ = help_functions::str2vec(str);
    return compute_value_x_content_type();
}

double host_based_features_t::compute_value_x_content_type() const
{
    const std::regex reg("content-type-options: nosniff", std::regex::icase);
    return check_value_in_output(http_resp_headers_, reg) ? 0 : 1;
}

double host_based_features_t::compute_value_asn(std::string_view str)
{
    asn_ = str;
    return compute_value_asn();
}

std::string host_based_features_t::get_asn() const
{
    // we don't have hostname -> IP address thus DNS record doesn't exist
    if (dig_response_.empty()) {
        return {};
    }
    // 1st line of dig is IP address for a hostname
    std::regex reg_ipv4("()(\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3})");
    auto ipv4 = extract_value_from_output(dig_response_, reg_ipv4);
    if (ipv4.empty()) {
        return {};
    }
    auto cmd = fmt::format("timeout {} whois --verbose {} | grep -i origin", 1, ipv4);
    auto output = help_functions::get_output_from_program(cmd);
    std::regex reg("(origin).* ([[:alnum:]]*)", std::regex::icase);
    return extract_value_from_output(output, reg);
}

double host_based_features_t::compute_value_asn() const
{
    // TODO: change to some meaningfull value after performing statistics
    return asn_.empty() ? 1 : 0;
}

void host_based_features_t::fill_asn()
{
    if (asn_.empty()) {
        asn_ = get_asn();
    }
}

std::string host_based_features_t::get_word_suggestion(std::string_view word) const
{
    auto cmd = fmt::format("curl --max-time 2 -s http://suggestqueries.google.com/complete/search?output=firefox\\&q={} | jq . | sed -n 4p | egrep -o '[[:alnum:]]*'",
                           word);
    return help_functions::get_line_from_program_if_exists(cmd, 0);
}

std::string host_based_features_t::get_similar_domain_name() const
{
    // we need to copmute sld beforehand
    if (sld_.empty()) {
        return "";
    }
    return get_word_suggestion(sld_);
}

void host_based_features_t::fill_similar_domain_name()
{
    similar_domain_ = get_similar_domain_name();
}

double host_based_features_t::compute_similar_domain() const
{
    const auto& suggestion = similar_domain_;
    // we got something unusual/unknown
    if (suggestion.empty()) {
        return 1;
    }
    // e.g. 'paypal == paypal'
    if (suggestion == sld_) {
        return 0;
    }
    // e.g. 'paypal <-> paypel'
    // compute levehstein
    auto result = edlibAlign(suggestion.c_str(), static_cast<int>(suggestion.size()),
                               sld_.c_str(), static_cast<int>(sld_.size()),
                               edlibDefaultAlignConfig());
    auto distance = result.editDistance;
    edlibFreeAlignResult(result);
    // fmt::print("{} - {} -- distance = {}\n", suggestion, sld_, distance);
    return 1. / static_cast<double>(distance);
}

void host_based_features_t::fill_sld()
{
    if (sld_.empty()) {
        sld_ = get_sld();
    }
}

std::string host_based_features_t::get_sld() const
{
    // https://github.com/stricaud/faup/issues/94#issuecomment-521689212
    auto cmd = fmt::format("echo {} | faup -f domain_without_tld", _parsed.getHost());
    return help_functions::get_line_from_program_if_exists(cmd, 0);
}

double host_based_features_t::compute_similar_domain(std::string_view str, std::string_view str2)
{
    similar_domain_ = str;
    sld_ = str2;
    return compute_similar_domain();
}

std::vector<std::string> host_based_features_t::extra_columns()
{
    std::vector<std::string> values;
    if (_flags & feature_enum::dns_created) {
        values.push_back(std::string(feature_enum::column_names.at(feature_enum::dns_created)) + "_value");
    }
    if (_flags & feature_enum::dns_updated) {
        values.push_back(std::string(feature_enum::column_names.at(feature_enum::dns_updated)) + "_value");
    }
    if (_flags & feature_enum::ssl_created) {
        values.push_back(std::string(feature_enum::column_names.at(feature_enum::ssl_created)) + "_value");
    }
    if (_flags & feature_enum::ssl_expire) {
        values.push_back(std::string(feature_enum::column_names.at(feature_enum::ssl_expire)) + "_value");
    }
    if (_flags & feature_enum::asn) {
        values.push_back(std::string(feature_enum::column_names.at(feature_enum::asn)) + "_value");
    }
    return values;
}

std::string host_based_features_t::extra_values()
{
    std::string values = "";
    if (_flags & feature_enum::dns_created) {
        values += get_dns_created() + ",";
    }
    if (_flags & feature_enum::dns_updated) {
        values += get_dns_updated() + ",";
    }
    if (_flags & feature_enum::ssl_created) {
        values += get_ssl_created() + ",";
    }
    if (_flags & feature_enum::ssl_expire) {
        values += get_ssl_expire() + ",";
    }
    if (_flags & feature_enum::asn) {
        values += asn_ + ",";
    }
    if (values.empty())
        return "";
    if (values.back() == ',') values.pop_back();
    return values;
}

double host_based_features_t::compute_value(feature_enum::id feature) const
{
    switch (feature) {
    // URL FEATURES IN ANOTHER FILE
    case feature_enum::ip_address:
    case feature_enum::url_length:
    case feature_enum::host_length:
    case feature_enum::path_length:
    case feature_enum::query_length:
    case feature_enum::fragment_length:
    case feature_enum::user_info:
    case feature_enum::domain_count:
    case feature_enum::https_used:
    case feature_enum::extra_https:
    case feature_enum::shortening_service:
    case feature_enum::non_std_port:
    case feature_enum::spec_chars_path:
    case feature_enum::spec_chars_query:
    case feature_enum::spec_chars_fragment:
    case feature_enum::spec_chars_host:
    case feature_enum::gtld:
    case feature_enum::www_prefix:
    case feature_enum::four_numbers:
    case feature_enum::spec_keywords:
    case feature_enum::punycode:
        return 0;
    // HTML FEATURES IN ANOTHER FILE
    case feature_enum::input_tag:
    case feature_enum::src_link:
    case feature_enum::form_handler:
    case feature_enum::invisible_iframe:
    case feature_enum::rewrite_statusbar:
    case feature_enum::disable_rightclick:
    case feature_enum::ahref_link:
    case feature_enum::popup_window:
    case feature_enum::favicon_link:
    case feature_enum::old_technologies:
    case feature_enum::missleading_link:
    case feature_enum::hostname_title:
        return 0;
    // HOST BASED FEATURES
    case feature_enum::redirect: return compute_value_redirect();
    case feature_enum::google_index: return compute_value_google_indexed();
    case feature_enum::dns_a_record: return compute_value_dns_a_record();
    case feature_enum::dnssec: return compute_value_dnssec();
    case feature_enum::dns_created: return compute_value_dns_created();
    case feature_enum::dns_updated: return compute_value_dns_updated();
    case feature_enum::ssl_created: return compute_value_ssl_created();
    case feature_enum::ssl_expire:  return compute_value_ssl_expire();
    case feature_enum::ssl_subject: return compute_value_ssl_subject();
    case feature_enum::hsts: return compute_value_hsts();
    case feature_enum::xss_protection: return compute_value_xss_protection();
    case feature_enum::csp: return compute_value_csp();
    case feature_enum::x_frame: return compute_value_x_frame();
    case feature_enum::x_content_type: return compute_value_x_content_type();
    case feature_enum::asn: return compute_value_asn();
    case feature_enum::similar_domain: return compute_similar_domain();
    }
    return 0;
}
