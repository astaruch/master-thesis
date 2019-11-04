#pragma once

#ifndef PHISHSVC_FEATURES_H
#define PHISHSVC_FEATURES_H

#include <string>
#include <string_view>
#include <unordered_map>

#include <Poco/URI.h>

class features_t {
public:
    features_t(const std::string& url);
    features_t(const std::string& url, uint64_t url_flags, uint64_t html_flags, int label);

    std::vector<double> compute_feature_vector() const;
    void set_html_features_opts(const std::string& node_bin, const std::string& html_script);
protected:
    std::string _url;
    uint64_t _url_flags;
    uint64_t _html_flags;
    int _label;
    Poco::URI _parsed;
    bool _url_is_ok;

    std::string _node_bin;
    std::string _html_script;
};

#endif // PHISHSVC_FEATURES_H
