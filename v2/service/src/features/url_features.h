#pragma once

#ifndef PHISHSVC_FEATURE_URL_FEATURES_H
#define PHISHSVC_FEATURE_URL_FEATURES_H

#include "feature_base.h"

#include <string>
#include <unordered_map>

#include <Poco/URI.h>

class url_features_t {
public:
    explicit url_features_t(const std::string_view url);
    url_features_t(const std::string_view url, const Poco::URI& parsed, const uint64_t flags,
        const bool url_is_ok);

    std::unordered_map<feature_enum::id, double> compute_values();
    double compute_value(feature_enum::id feature);
    double compute_value_ip_address();
    double compute_value_url_length() const;
    double compute_value_url_length(int min, int max) const;
    double compute_value_host_length() const;
    double compute_value_host_length(int min, int max) const;
    double compute_value_path_length() const;
    double compute_value_path_length(int min, int max) const;
    double compute_value_query_length() const;
    double compute_value_query_length(int min, int max) const;
    double compute_value_fragment_length() const;
    double compute_value_fragment_length(int min, int max) const;
    double compute_value_user_info() const;
private:
    const std::string_view _url;
    const Poco::URI _parsed{};
    const uint64_t _flags{0};
    const bool _url_is_ok{false};
};

#endif // PHISHSVC_FEATURE_URL_FEATURES_H
