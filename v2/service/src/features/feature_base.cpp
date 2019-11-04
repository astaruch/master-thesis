#include "feature_base.h"

#include <Poco/Exception.h>
#include <fmt/format.h>

namespace feature {

const std::unordered_map<feature_enum::id, std::string_view> base::column_names = base::create_column_names();

std::unordered_map<feature_enum::id, std::string_view> base::create_column_names()
{
    std::unordered_map<feature_enum::id, std::string_view> map{
        // URL
        { feature_enum::ip_address, "ip_address" },
        { feature_enum::url_length, "url_length" },
        { feature_enum::host_length, "host_length" },
        { feature_enum::path_length, "path_length" },
        { feature_enum::query_length, "query_length" },
        { feature_enum::fragment_length, "fragment_length" },
        { feature_enum::user_info, "user_info" },
        { feature_enum::domain_count, "domain_count" },
        { feature_enum::https_used, "https_used" },
        { feature_enum::extra_https, "extra_https" },
        { feature_enum::shortening_service, "shortening_service" },
        { feature_enum::non_std_port, "non_std_port" },
        { feature_enum::spec_chars_path, "spec_chars_path" },
        { feature_enum::spec_chars_query, "spec_chars_query" },
        { feature_enum::spec_chars_fragment, "spec_chars_fragment" },
        { feature_enum::spec_chars_host, "spec_chars_host" },
        { feature_enum::gtld, "gtld" },
        { feature_enum::www_prefix, "www_prefix" },
        { feature_enum::four_numbers, "four_numbers" },
        { feature_enum::spec_keywords, "spec_keywords" },
        { feature_enum::punycode, "punycode" },
        // HTML
        { feature_enum::input_tag, "input_tag" },
        { feature_enum::src_link, "src_link" },
        { feature_enum::form_handler, "form_handler" },
        { feature_enum::invisible_iframe, "invisible_iframe" },
        { feature_enum::rewrite_statusbar, "rewrite_statusbar" },
        { feature_enum::disable_rightclick, "disable_rightclick" },
        { feature_enum::ahref_link, "ahref_link" },
        { feature_enum::popup_window, "popup_window" },
        { feature_enum::favicon_link, "favicon_link" },
        { feature_enum::old_technologies, "old_technologies" },
        { feature_enum::missleading_link, "missleading_link" },
        { feature_enum::hostname_title, "hostname_title" },
        // HOST-BASED
        { feature_enum::redirect, "redirect" },
    };
    return map;
}

void base::set_url(const std::string& url)
{
    _url = url;
    try {
        _url_obj = Poco::URI(url);
        _url_is_ok = true;
    } catch (const Poco::SyntaxException& ex) {
        fmt::print(stderr, "Error. URL '{}' is malformed.\n", url);
        _url_obj = Poco::URI();
        _value = 1.0; // we say that URL is phishing if we can't parse it
    }
}

} // namespace feature

