#include "url_features.h"

#include "../help_functions.h"

#include <algorithm>
#include <regex>

#include <fmt/format.h>
#include <Poco/Exception.h>

namespace phishscore {

url_features_t::url_features_t(const std::string_view url)
try
   : _url(url)
   , _parsed(Poco::URI(url.begin()))
   , _url_is_ok(true)
{
}
catch (const Poco::SyntaxException& ex)
{
    fmt::print(stderr, "{}: {}\n", ex.what(), ex.message());
    // we didn't finish parsing an URL thus _url_is_ok is still false
}

url_features_t::url_features_t(const std::string_view url, const Poco::URI& parsed, const uint64_t flags,
        const bool url_is_ok)
    : _url(url)
    , _flags(flags)
    , _parsed(parsed)
    , _url_is_ok(url_is_ok)
{
}

std::unordered_map<std::string_view, double> url_features_t::compute_values_map() const
{
    std::unordered_map<std::string_view, double> values;
    for (const auto feature: feature_enum::url) {
        if (_flags & feature) {
            values[feature_enum::column_names.at(feature)] = compute_value(feature);
        }
    }
    return values;
}

std::vector<double> url_features_t::compute_values_vec() const
{
    std::vector<double> values;
    for (const auto feature: feature_enum::url) {
        if (_flags & feature) {
            values.push_back(compute_value(feature));
        }
    }
    return values;
}

double url_features_t::compute_value_ip_address() const
{
    /**************************
     * Explanation of the regex
    ^
        (?:
          # Dotted variants:
          (?:
            # Decimal 1-255 (no leading 0's)
            [3-9]\d?|2(?:5[0-5]|[0-4]?\d)?|1\d{0,2}
          |
            0x0*[0-9a-f]{1,2}  # Hexadecimal 0x0 - 0xFF (possible leading 0's)
          |
            0+[1-3]?[0-7]{0,2} # Octal 0 - 0377 (possible leading 0's)
          )
          (?:                  # Repeat 0-3 times, separated by a dot
            \.
            (?:
              [3-9]\d?|2(?:5[0-5]|[0-4]?\d)?|1\d{0,2}
            |
              0x0*[0-9a-f]{1,2}
            |
              0+[1-3]?[0-7]{0,2}
            )
          ){0,3}
        |
          0x0*[0-9a-f]{1,8}    # Hexadecimal notation, 0x0 - 0xffffffff
        |
          0+[0-3]?[0-7]{0,10}  # Octal notation, 0 - 037777777777
        |
          # Decimal notation, 1-4294967295:
          429496729[0-5]|42949672[0-8]\d|4294967[01]\d\d|429496[0-6]\d{3}|
          42949[0-5]\d{4}|4294[0-8]\d{5}|429[0-3]\d{6}|42[0-8]\d{7}|
          4[01]\d{8}|[1-3]\d{0,9}|[4-9]\d{0,8}
        )
    $
    */
    std::string pattern = R"(^
        (?:
          (?:
            [3-9]\d?|2(?:5[0-5]|[0-4]?\d)?|1\d{0,2}
          |
            0x0*[0-9a-f]{1,2}
          |
            0+[1-3]?[0-7]{0,2}
          )
          (?:
            \.
            (?:
              [3-9]\d?|2(?:5[0-5]|[0-4]?\d)?|1\d{0,2}
            |
              0x0*[0-9a-f]{1,2}
            |
              0+[1-3]?[0-7]{0,2}
            )
          ){0,3}
        |
          0x0*[0-9a-f]{1,8}
        |
          0+[0-3]?[0-7]{0,10}
        |
          429496729[0-5]|42949672[0-8]\d|4294967[01]\d\d|429496[0-6]\d{3}|
          42949[0-5]\d{4}|4294[0-8]\d{5}|429[0-3]\d{6}|42[0-8]\d{7}|
          4[01]\d{8}|[1-3]\d{0,9}|[4-9]\d{0,8}
        )$
    )";
    pattern.erase(remove_if(pattern.begin(), pattern.end(), isspace), pattern.end());
    const std::regex ip_regex(pattern);
    return std::regex_match(_parsed.getHost(), ip_regex) ? 1. : 0.;
}

double url_features_t::compute_value_url_length() const
{
    return compute_value_url_length(14, 161);
}

double url_features_t::compute_value_url_length(int min, int max) const
{
    return help_functions::normalize_value(min, _url.length(), max);
}

double url_features_t::compute_value_host_length() const
{
    return compute_value_host_length(10, 37);
}

double url_features_t::compute_value_host_length(int min, int max) const
{
    return help_functions::normalize_value(min, _parsed.getHost().length(), max);
}

double url_features_t::compute_value_path_length() const
{
    return compute_value_path_length(1, 89);
}

double url_features_t::compute_value_path_length(int min, int max) const
{
    return help_functions::normalize_value(min, _parsed.getPath().length(), max);
}

double url_features_t::compute_value_query_length() const
{
    return compute_value_query_length(6, 220);
}

double url_features_t::compute_value_query_length(int min, int max) const
{
    return help_functions::normalize_value(min, _parsed.getRawQuery().length(), max);
}

double url_features_t::compute_value_fragment_length() const
{
    return compute_value_fragment_length(1, 155);
}

double url_features_t::compute_value_fragment_length(int min, int max) const
{
    return help_functions::normalize_value(min, _parsed.getFragment().length(), max);
}

double url_features_t::compute_value_user_info() const
{
    return _parsed.getUserInfo().empty()? 0 : 1;
}

double url_features_t::compute_value_domain_count() const
{
    return compute_value_domain_count(1, 21);
}

double url_features_t::compute_value_domain_count(int min, int max) const
{
    const auto& host = _parsed.getHost();
    int count = static_cast<int>(std::count(host.begin(), host.end(), '.'));
    return help_functions::normalize_value(min, count, max);
}

double url_features_t::compute_value_https_used() const
{
    auto scheme = _parsed.getScheme();
    std::transform(scheme.begin(), scheme.end(), scheme.begin(), tolower);
    return scheme == "https" ? 0 : 1;
}

double url_features_t::compute_value_extra_https() const
{
    auto host = _parsed.getHost();
    std::transform(host.begin(), host.end(), host.begin(), tolower);
    if (host.find("https") == 0 || host.find("www.https") == 0) {
        return 1;
    }
    return 0;
}

double url_features_t::compute_value_shortening_service() const
{
    const std::regex reg("(bitly.|bit.ly|tinyurl.com|goo.gl|tiny.cc|livechatinc.com|is.gd|soo.gd|clicky.me|cutt.ly)");
    return std::regex_search(_parsed.getHost(), reg);
}

double url_features_t::compute_value_non_std_port() const
{
    const auto& port = _parsed.getPort();
    return (port != 80 && port != 443) ? 1 : 0;
}

double url_features_t::compute_value_spec_char_host() const
{
    return compute_value_spec_char_host(0, 2);
}

double url_features_t::compute_value_spec_char_host(int min, int max) const
{
    const auto& str = _parsed.getHost();
    auto count = std::count_if(str.begin(), str.end(), [](const char c) {
        return c == '_' || c == '-';
    });
    return help_functions::normalize_value(min, count, max);
}

double url_features_t::compute_value_spec_char_path() const
{
    return compute_value_spec_char_path(1, 8);
}

double url_features_t::compute_value_spec_char_path(int min, int max) const
{
    const auto& str = _parsed.getPath();
    auto count = std::count_if(str.begin(), str.end(), ::ispunct);
    return help_functions::normalize_value(min, count, max);
}

double url_features_t::compute_value_spec_char_query() const
{
    return compute_value_spec_char_query(0, 13);
}

double url_features_t::compute_value_spec_char_query(int min, int max) const
{
    const auto& str = _parsed.getRawQuery();
    auto count = std::count_if(str.begin(), str.end(), ::ispunct);
    return help_functions::normalize_value(min, count, max);
}

double url_features_t::compute_value_spec_char_fragment() const
{
    return compute_value_spec_char_fragment(0, 5);
}

double url_features_t::compute_value_spec_char_fragment(int min, int max) const
{
    const auto& str = _parsed.getFragment();
    auto count = std::count_if(str.begin(), str.end(), ::ispunct);
    return help_functions::normalize_value(min, count, max);
}

double url_features_t::compute_value_gtld() const
{
    const std::regex reg("(.com|.org|.net|.int|.edu|.gov|.mil|.cz|.sk)$");
    return std::regex_search(_parsed.getHost(), reg) ? 0 : 1;
}

double url_features_t::compute_value_www_prefix() const
{
    const std::regex reg("^(www-)");
    return std::regex_search(_parsed.getHost(), reg) ? 1 : 0;
}

double url_features_t::compute_value_four_numbers() const
{
    const std::regex reg("[[:digit:]]{4}");
    return std::regex_search(_parsed.getHost(), reg) ? 1 : 0;
}

double url_features_t::compute_value_punycode() const
{
    return _parsed.getHost().find("xn--") == std::string::npos ? 0 : 1;
}

double url_features_t::compute_value_spec_keywords() const
{
    return compute_value_spec_keywords(0, 10);
}

double url_features_t::compute_value_spec_keywords(int min, int max) const
{
    std::regex reg("secure|account|webscr|login|ebayisapi|signin|banking|confirm", std::regex::icase | std::regex::optimize);
    std::smatch match;
    int count = 0;
    std::string url(_url);
    while (std::regex_search(url, match, reg)) {
        count++;
        url = match.suffix();
    }
    return help_functions::normalize_value(min, count, max);
}

double url_features_t::compute_value(feature_enum::id feature) const
{
    // if we couldn't parse an URL, we are marking all features as phishy
    if (!_url_is_ok) {
        return -1.;
    }
    switch (feature) {
    case feature_enum::ip_address: return compute_value_ip_address();
    case feature_enum::url_length: return compute_value_url_length();
    case feature_enum::host_length: return compute_value_host_length();
    case feature_enum::path_length: return compute_value_path_length();
    case feature_enum::query_length: return compute_value_query_length();
    case feature_enum::fragment_length: return compute_value_fragment_length();
    case feature_enum::user_info: return compute_value_user_info();
    case feature_enum::domain_count: return compute_value_domain_count();
    case feature_enum::https_used: return compute_value_https_used();
    case feature_enum::extra_https: return compute_value_extra_https();
    case feature_enum::shortening_service: return compute_value_shortening_service();
    case feature_enum::non_std_port: return compute_value_non_std_port();
    case feature_enum::spec_chars_path: return compute_value_spec_char_path();
    case feature_enum::spec_chars_query: return compute_value_spec_char_query();
    case feature_enum::spec_chars_fragment: return compute_value_spec_char_fragment();
    case feature_enum::spec_chars_host: return compute_value_spec_char_host();
    case feature_enum::gtld: return compute_value_gtld();
    case feature_enum::www_prefix: return compute_value_www_prefix();
    case feature_enum::four_numbers: return compute_value_four_numbers();
    case feature_enum::spec_keywords: return compute_value_spec_keywords();
    case feature_enum::punycode: return compute_value_punycode();
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
    // HOST BASED FEATURES IN ANOTHER FILE
    case feature_enum::redirect:
    case feature_enum::google_index:
    case feature_enum::dns_a_record:
    case feature_enum::dnssec:
    case feature_enum::dns_created:
    case feature_enum::dns_updated:
    case feature_enum::ssl_created:
    case feature_enum::ssl_expire:
    case feature_enum::ssl_subject:
    case feature_enum::hsts:
    case feature_enum::xss_protection:
    case feature_enum::csp:
    case feature_enum::x_frame:
    case feature_enum::x_content_type:
    case feature_enum::asn:
    case feature_enum::similar_domain:
        return 0;
    }
    return 0;
}

} // namespace phishscore
