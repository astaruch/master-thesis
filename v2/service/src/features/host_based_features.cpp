#include "host_based_features.h"

#include "../help_functions.h"

#include <algorithm>
#include <regex>
#include <string_view>

#include <fmt/format.h>
#include <Poco/Exception.h>

host_based_features_t::host_based_features_t(const std::string_view url)
try
   : _url(url)
   , _parsed(Poco::URI(url.begin()))
   , _url_is_ok(true)
{
}
catch (const Poco::SyntaxException& ex)
{
    // we didn't finish parsing an URL thus _url_is_ok is still false
}

host_based_features_t::host_based_features_t(const std::string_view url,
                                             const Poco::URI& parsed,
                                             const uint64_t flags,
                                             const bool url_is_ok)
    : _url(url)
    , _flags(flags)
    , _parsed(parsed)
    , _url_is_ok(url_is_ok)
{
    if (_flags & (feature_enum::ssl_created | feature_enum::ssl_expire | feature_enum::ssl_subject)) {
        ssl_response_ = get_ssl_response();
    }
    if (_flags & (feature_enum::hsts | feature_enum::xss_protection |
                  feature_enum::csp | feature_enum::x_content_type))
    {
        http_resp_headers_ = get_http_resp_headers();
    }
    if (_flags & (feature_enum::dns_a_record | feature_enum::dnssec | feature_enum::asn)) {
        dig_response_ = get_dig_response();
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

double host_based_features_t::compute_value_redirect() const
{
    auto cmd = fmt::format("curl -s -w \"%{{http_code}}\" -o /dev/null {}", _url);
    auto output = help_functions::get_output_from_program(cmd);
    auto http_code_str = output.front();
    try {
        auto http_code = std::stoi(http_code_str); // throws invalid_exception
        return http_code == 301 ? 1 : 0;
    } catch (const std::invalid_argument& ex) {
        return 0;
    }
    return 0;
}

double host_based_features_t::compute_value_google_indexed() const
{
    auto cmd = fmt::format("curl -s 'https://www.google.com/search?hl=en&q=site%3A{}' -H '{}' --compressed | grep 'did not match any documents'",
                           _parsed.getHost(), _user_agent);
    auto output = help_functions::get_output_from_program(cmd);
    return output.empty() ? 0 : 1;
}

std::vector<std::string> host_based_features_t::get_dig_response() const
{
    auto cmd = fmt::format("dig +dnssec +short {}", _parsed.getHost());
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

double host_based_features_t::compute_value_dns_a_record(bool)
{
    fill_dig_response();
    return compute_value_dns_a_record();
}

double host_based_features_t::compute_value_dnssec() const
{
    return dig_response_.size() > 1 ? 0 : 1;
}

double host_based_features_t::compute_value_dnssec(bool)
{
    fill_dig_response();
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

std::string host_based_features_t::extract_dns_date(bool created) const
{
    auto cmd = fmt::format("whois {}", _parsed.getHost());
    auto output = help_functions::get_output_from_program(cmd);
    auto reg_created = R"((creat|regist)(?:.*)(\d{8}|\d{4}\/\d{2}\/\d{2}|\d{4}-\d{2}-\d{2}|\d{2}\.\d{2}\.\d{4}))";
    auto reg_updated = R"((chang|updat)(?:.*)(\d{8}|\d{4}\/\d{2}\/\d{2}|\d{4}-\d{2}-\d{2}|\d{2}\.\d{2}\.\d{4}))";
    const std::regex reg(created ? reg_created : reg_updated, std::regex::icase);
    auto value = extract_value_from_output(output, reg);
        if (value.find('.') != std::string::npos) {
        value = fmt::format("{}-{}-{}", value.substr(6,4), value.substr(3,2), value.substr(0,2));
    }
    value.erase(remove_if(value.begin(), value.end(), ispunct), value.end());
    return value;
}

double host_based_features_t::compute_value_dns_created() const
{
    auto created = extract_dns_date(true);
    if (created.empty()) {
        // could not find basic DNS record in traditional format
        return 1.;
    }
    return help_functions::normalize_date_string(created);}

double host_based_features_t::compute_value_dns_updated() const
{
    auto created = extract_dns_date(false);
    if (created.empty()) {
        // could not find basic DNS record in traditional format
        return 1.;
    }
    return help_functions::normalize_date_string(created);
}

std::vector<std::string> host_based_features_t::get_ssl_response() const
{
    auto cmd = fmt::format("echo | timeout 2 openssl s_client -connect {}:{} 2>/dev/null | openssl x509 -noout -subject -dates 2>/dev/null ",
                           _parsed.getHost(), _parsed.getPort());
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

double host_based_features_t::compute_value_ssl_created(bool)
{
    fill_ssl_response();
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

double host_based_features_t::compute_value_ssl_expire(bool)
{
    fill_ssl_response();
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

double host_based_features_t::compute_value_ssl_subject(bool)
{
    fill_ssl_response();
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
    auto cmd = fmt::format("curl -s -I -X GET {}", _url);
    return help_functions::get_output_from_program(cmd);
}

double host_based_features_t::compute_value_hsts(bool)
{
    fill_http_resp_headers();
    return compute_value_hsts();
}

double host_based_features_t::compute_value_hsts() const
{
    const std::regex reg("strict-transport-security", std::regex::icase);
    return check_value_in_output(http_resp_headers_, reg) ? 0 : 1;
}

double host_based_features_t::compute_value_xss_protection(bool)
{
    fill_http_resp_headers();
    return compute_value_xss_protection();
}

double host_based_features_t::compute_value_xss_protection() const
{
    const std::regex reg("x-xss-protection:.*1", std::regex::icase);
    return check_value_in_output(http_resp_headers_, reg) ? 0 : 1;
}

double host_based_features_t::compute_value_csp(bool)
{
    fill_http_resp_headers();
    return compute_value_csp();
}

double host_based_features_t::compute_value_csp() const
{
    const std::regex reg("content-security-policy", std::regex::icase);
    return check_value_in_output(http_resp_headers_, reg) ? 0 : 1;
}

double host_based_features_t::compute_value_x_frame(bool)
{
    fill_http_resp_headers();
    return compute_value_x_frame();
}

double host_based_features_t::compute_value_x_frame() const
{
    const std::regex reg("x-frame-options:.*(sameorigin|deny)", std::regex::icase);
    return check_value_in_output(http_resp_headers_, reg) ? 0 : 1;
}

double host_based_features_t::compute_value_x_content_type(bool)
{
    fill_http_resp_headers();
    return compute_value_x_content_type();
}

double host_based_features_t::compute_value_x_content_type() const
{
    const std::regex reg("content-type-options: nosniff", std::regex::icase);
    return check_value_in_output(http_resp_headers_, reg) ? 0 : 1;
}

double host_based_features_t::compute_value_asn(bool)
{
    fill_dig_response();
    return compute_value_asn();
}

std::string host_based_features_t::get_asn() const
{
    // we don't have hostname -> IP address thus DNS record doesn't exist
    if (dig_response_.empty()) {
        return {};
    }
    // 1st line of dig is IP address for a hostname
    auto cmd = fmt::format("whois --verbose {} | grep -i origin", dig_response_[0]);
    auto output = help_functions::get_output_from_program(cmd);
    std::regex reg("(origin).* ([[:alnum:]]*)", std::regex::icase);
    return extract_value_from_output(output, reg);
}

double host_based_features_t::compute_value_asn() const
{
    // TODO: change to some meaningfull value after performing statistics
    return get_asn().empty() ? 1 : 0;
}

double host_based_features_t::compute_value(feature_enum::id feature) const
{
    // if we couldn't parse an URL, we are marking all features as phishy
    if (!_url_is_ok) {
        return 1.;
    }
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
    case feature_enum::similar_domain:
        return 0;
    }
    return 0;
}
