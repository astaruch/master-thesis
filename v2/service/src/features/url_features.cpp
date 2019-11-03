#include "url_features.h"

#include "../help_functions.h"

#include <algorithm>
#include <regex>

#include <Poco/Exception.h>

url_features_t::url_features_t(const std::string_view url)
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

url_features_t::url_features_t(const std::string_view url, const Poco::URI& parsed, const uint64_t flags,
        const bool url_is_ok)
    : _url(url)
    , _parsed(parsed)
    , _flags(flags)
    , _url_is_ok(url_is_ok)
{
}

std::unordered_map<feature_enum::id, double> url_features_t::compute_values()
{
    std::unordered_map<feature_enum::id, double> values;
    for (const auto feature: feature_enum::url) {
        if (_flags & feature) {
            values[feature] = compute_value(feature);
        }
    }
    return values;
}

double url_features_t::compute_value_ip_address()
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

double url_features_t::compute_value_url_length()
{
    return compute_value_url_length(14, 161);
}

double url_features_t::compute_value_url_length(int min, int max)
{
    return help_functions::normalize_value(min, _url.length(), max);
}

double url_features_t::compute_value(feature_enum::id feature)
{
    // if we couldn't parse an URL, we are marking all features as phishy
    if (!_url_is_ok) {
        return 1.;
    }
    switch (feature) {
    case feature_enum::ip_address: return compute_value_ip_address();
    case feature_enum::url_length: return compute_value_url_length();
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
    case feature_enum::redirect:
        return 0;
    }
    return 0;
}
