#pragma once

#ifndef PHISHSVC_BASE_FEATURE_H
#define PHISHSVC_BASE_FEATURE_H

#include <string>
#include <string_view>
#include <unordered_map>

#include <Poco/URI.h>


namespace feature_enum {

enum id: uint64_t {
    // 0 bits
    ip_address =    0x1,
    url_length =    0x2,
    host_length =   0x4,
    path_length =   0x8,
    // 4 bits
    query_length =      0x10,
    fragment_length =   0x20,
    user_info =         0x40,
    domain_count =      0x80,
    // 8 bits
    https_used =            0x100,
    extra_https =           0x200,
    shortening_service =    0x400,
    non_std_port =          0x800,
    // 12 bits
    spec_chars_path =       0x1000,
    spec_chars_query =      0x2000,
    spec_chars_fragment =   0x4000,
    spec_chars_host =       0x8000,
    // 16 bits
    gtld =          0x10000,
    www_prefix =    0x20000,
    four_numbers =  0x40000,
    spec_keywords = 0x80000,
    // 20 bits
    punycode =  0x100000,
    input_tag = 0x200000,
    src_link =  0x400000,
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
    src_link
};

}

namespace feature {

class base {
public:
    virtual ~base() = default;
    virtual double compute_value() = 0;
    virtual std::string column_name() = 0;
    void set_url(const std::string& url);
    // compute normalized value that goes into [0, 1] interval
    double normalize_value(int min, int value, int max);

    static std::unordered_map<feature_enum::id, std::string_view> create_column_names();
    static const std::unordered_map<feature_enum::id, std::string_view> column_names;
protected:
    double _value;
    std::string _url;
    Poco::URI _url_obj;
    bool _url_is_ok;
};

} // namespace feature

#endif // PHISHSVC_BASE_FEATURE_H
