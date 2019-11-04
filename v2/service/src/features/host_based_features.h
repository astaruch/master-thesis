#pragma once

#ifndef PHISHSVC_FEATURE_HOST_BASED_FEATURES_H
#define PHISHSVC_FEATURE_HOST_BASED_FEATURES_H

#include "feature_enum.h"

#include <string_view>
#include <vector>

#include <Poco/URI.h>

class host_based_features_t {
public:
    host_based_features_t(const std::string_view url, const Poco::URI& parsed,
        const uint64_t flags, const bool url_is_ok);

    std::vector<double> compute_values_vec() const;
    std::unordered_map<feature_enum::id, double> compute_values_map() const;
    double compute_value(feature_enum::id feature) const;
    double compute_value_redirect() const;
private:
    const std::string_view _url;
    const uint64_t _flags{0};
    const Poco::URI& _parsed{};
    const bool _url_is_ok{false};
};

#endif // PHISHSVC_FEATURE_HOST_BASED_FEATURES_H
