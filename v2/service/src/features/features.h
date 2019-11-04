#pragma once

#ifndef PHISHSVC_FEATURES_H
#define PHISHSVC_FEATURES_H

#include <string>
#include <string_view>
#include <unordered_map>

#include <Poco/URI.h>

class features_t {
public:
    void set_url(const std::string& url);
protected:
    double _value;
    std::string _url;
    Poco::URI _url_obj;
    bool _url_is_ok;
};

#endif // PHISHSVC_FEATURES_H
