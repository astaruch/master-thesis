#pragma once

#ifndef PHISHSVC_FEATURES_H
#define PHISHSVC_FEATURES_H

#include <string>
#include <string_view>
#include <unordered_map>

#include <Poco/URI.h>

class features_t {
public:
    features_t(const std::string& url, bool verbose);
    features_t(const std::string& url, uint64_t url_flags, uint64_t html_flags,
               uint64_t host_based_flags, int label, bool verbose);

    std::vector<double> compute_feature_vector();
    std::string compute_extra_values() const;
    void set_html_features_opts(const std::string& node_bin, const std::string& html_script, const std::string& htmlfeatures_bin);
protected:
    std::string _url;
    uint64_t _url_flags;
    uint64_t _html_flags;
    uint64_t _host_based_flags;
    int _label;
    bool _url_is_ok;
    bool verbose_{false};

    Poco::URI _parsed;

    std::string extra_values_{};

    std::string node_bin_;
    std::string html_script_;
    std::string htmlfeatures_bin_;
};

#endif // PHISHSVC_FEATURES_H
