#pragma once

#ifndef PHISHSVC_BASE_FEATURE_H
#define PHISHSVC_BASE_FEATURE_H

#include <string>

#include <Poco/URI.h>

enum feature_name: uint64_t {
    ip_address = 0x1,
    url_length = 0x2,
    host_length = 0x4,
    path_length = 0x8,
    query_length = 0x10,
    fragment_length = 0x20,
    user_info = 0x40,
    domain_count = 0x80,
};

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
