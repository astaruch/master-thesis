#include "host_based_features.h"

host_based_features_t::host_based_features_t(const std::string_view url,
                                             const Poco::URI& parsed,
                                             const uint64_t flags,
                                             const bool url_is_ok)
    : _url(url)
    , _flags(flags)
    , _parsed(parsed)
    , _url_is_ok(url_is_ok)
{
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
    return 0;
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
    }
    return 0;
}
