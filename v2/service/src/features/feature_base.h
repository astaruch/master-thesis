#pragma once

#ifndef PHISHSVC_BASE_FEATURE_H
#define PHISHSVC_BASE_FEATURE_H

#include <string>

enum feature_name: uint64_t {
    ip_address = 1,
};

namespace feature {

class base {
public:
    virtual double compute_value() = 0;
    void set_url(const std::string& url);
protected:
    double _value;
    std::string _url;
};

} // namespace feature

#endif // PHISHSVC_BASE_FEATURE_H
