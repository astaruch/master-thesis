#pragma once

#ifndef PHISHSVC_FEATURE_ENUM_H
#define PHISHSVC_FEATURE_ENUM_H

#include <string_view>
#include <unordered_map>

namespace feature_enum {

enum id: uint64_t {
    // 0 bits
    ip_address =    0x1,
    url_length =    0x2,
    host_length =   0x4,
    path_length =   0x8,
    // 4 bits (1)
    query_length =      0x10,
    fragment_length =   0x20,
    user_info =         0x40,
    domain_count =      0x80,
    // 8 bits (2)
    https_used =            0x100,
    extra_https =           0x200,
    shortening_service =    0x400,
    non_std_port =          0x800,
    // 12 bits (3)
    spec_chars_path =       0x1000,
    spec_chars_query =      0x2000,
    spec_chars_fragment =   0x4000,
    spec_chars_host =       0x8000,
    // 16 bits (4)
    gtld =          0x10000,
    www_prefix =    0x20000,
    four_numbers =  0x40000,
    spec_keywords = 0x80000,
    // 20 bits (5)
    punycode =     0x100000,
    //// HTML FEATURES
    input_tag =    0x200000,
    src_link =     0x400000,
    form_handler = 0x800000,
    // 24 bits (6)
    invisible_iframe =   0x1000000,
    rewrite_statusbar =  0x2000000,
    disable_rightclick = 0x4000000,
    ahref_link =         0x8000000,
    // 38 bits (7)
    popup_window =     0x10000000,
    favicon_link =     0x20000000,
    old_technologies = 0x40000000,
    missleading_link = 0x80000000,
    // 32 bits (8)
    hostname_title = 0x100000000,
    //// HOST BASED FEATURES
    redirect =       0x200000000,
    google_index =   0x400000000,
    dns_a_record =    0x800000000,
    // 36 bits (9)
    dnssec =      0x1000000000,
    dns_created = 0x2000000000,
    dns_updated = 0x4000000000,
};

static const id url[] = {
    ip_address,
    url_length,
    host_length,
    path_length,
    query_length,
    fragment_length,
    user_info,
    domain_count,
    https_used,
    extra_https,
    shortening_service,
    non_std_port,
    spec_chars_path,
    spec_chars_query,
    spec_chars_fragment,
    spec_chars_host,
    gtld,
    www_prefix,
    four_numbers,
    spec_keywords,
    punycode,
};

static const id html[] = {
    input_tag,
    src_link,
    form_handler,
    invisible_iframe,
    rewrite_statusbar,
    disable_rightclick,
    ahref_link,
    popup_window,
    favicon_link,
    old_technologies,
    missleading_link,
    hostname_title,
};

static const id host_based[] = {
    redirect,
    google_index,
    dns_a_record,
    dnssec,
    dns_created,
    dns_updated,
};

static const std::unordered_map<feature_enum::id, std::string_view> column_names{
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
    { feature_enum::google_index, "google_index" },
    { feature_enum::dns_a_record, "dns_a_record" },
    { feature_enum::dnssec, "dnssec" },
    { feature_enum::dns_created, "dns_created" },
    { feature_enum::dns_updated, "dns_updated" },
};

} // namespace feature_enum

#endif // PHISHSVC_FEATURE_ENUM_H
