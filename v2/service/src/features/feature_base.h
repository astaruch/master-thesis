#pragma once

#ifndef PHISHSVC_BASE_FEATURE_H
#define PHISHSVC_BASE_FEATURE_H

#include <string>

#include <Poco/URI.h>

enum feature_name: uint64_t {
    ip_address = 1,
    url_length = 2,
};

namespace feature {

class base {
public:
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
