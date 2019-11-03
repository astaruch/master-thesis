#include "all_features.h"

namespace feature {

std::unique_ptr<feature::base> feature::creator::create_feature_from_flag(feature_enum::id id)
{
    switch(id) {
    case feature_enum::id::ip_address:
        return std::make_unique<feature::ip_address>();
    case feature_enum::id::url_length:
        return std::make_unique<feature::url_length>();
    case feature_enum::id::host_length:
        return std::make_unique<feature::host_length>();
    case feature_enum::id::path_length:
        return std::make_unique<feature::path_length>();
    case feature_enum::id::query_length:
        return std::make_unique<feature::query_length>();
    case feature_enum::id::fragment_length:
        return std::make_unique<feature::fragment_length>();
    case feature_enum::id::user_info:
        return std::make_unique<feature::user_info>();
    case feature_enum::id::domain_count:
        return std::make_unique<feature::domain_count>();
    case feature_enum::id::https_used:
        return std::make_unique<feature::https_used>();
    case feature_enum::id::extra_https:
        return std::make_unique<feature::extra_https>();
    case feature_enum::id::shortening_service:
        return std::make_unique<feature::shortening_service>();
    case feature_enum::id::non_std_port:
        return std::make_unique<feature::non_std_port>();
    case feature_enum::id::spec_chars_path:
        return std::make_unique<feature::spec_char_path>();
    case feature_enum::id::spec_chars_query:
        return std::make_unique<feature::spec_char_query>();
    case feature_enum::id::spec_chars_fragment:
        return std::make_unique<feature::spec_char_fragment>();
    case feature_enum::id::spec_chars_host:
        return std::make_unique<feature::spec_char_host>();
    case feature_enum::id::gtld:
        return std::make_unique<feature::gtld>();
    case feature_enum::id::www_prefix:
        return std::make_unique<feature::www_prefix>();
    case feature_enum::id::four_numbers:
        return std::make_unique<feature::four_numbers>();
    case feature_enum::id::spec_keywords:
        return std::make_unique<feature::spec_keywords>();
    case feature_enum::id::punycode:
        return std::make_unique<feature::punycode>();
    // we are returning nothing because HTML features are handling in another way
    case feature_enum::id::input_tag:
    case feature_enum::id::src_link:
    case feature_enum::id::form_handler:
    case feature_enum::id::invisible_iframe:
    case feature_enum::id::rewrite_statusbar:
    case feature_enum::id::disable_rightclick:
    case feature_enum::id::ahref_link:
    case feature_enum::id::popup_window:
    case feature_enum::id::favicon_link:
    case feature_enum::id::old_technologies:
    case feature_enum::id::missleading_link:
    case feature_enum::id::hostname_title:
        return nullptr;
    }
    return nullptr;
}

} // namespace feature
