#pragma once

#ifndef PHISHSVC_BASE_FEATURE_H
#define PHISHSVC_BASE_FEATURE_H

#include <string>
#include <string_view>
#include <unordered_map>

#include <Poco/URI.h>

namespace feature {

class base {
public:
    void set_url(const std::string& url);
protected:
    double _value;
    std::string _url;
    Poco::URI _url_obj;
    bool _url_is_ok;
};

} // namespace feature

#endif // PHISHSVC_BASE_FEATURE_H
