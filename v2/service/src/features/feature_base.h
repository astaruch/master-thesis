#pragma once

#ifndef PHISHSVC_BASE_FEATURE_H
#define PHISHSVC_BASE_FEATURE_H

#include <string>

#include <Poco/URI.h>

namespace feature_enum {

enum name: uint64_t {
    ip_address = 0x1,
    url_length = 0x2,
    host_length = 0x4,
    path_length = 0x8,
    // 4 bits
    query_length = 0x10,
    fragment_length = 0x20,
    user_info = 0x40,
    domain_count = 0x80,
    // 8 bits
    https_used = 0x100,
};

static const name all[] = {
    ip_address,
    url_length,
    host_length,
    path_length,
    query_length,
    fragment_length,
    user_info,
    domain_count,
    https_used,
};

}

namespace feature {

class base {
public:
    virtual ~base() = default;
    virtual double compute_value() = 0;
    void set_url(const std::string& url);
protected:
    double _value;
    std::string _url;
    Poco::URI _url_obj;
    bool _url_is_ok;
};

} // namespace feature

#endif // PHISHSVC_BASE_FEATURE_H
