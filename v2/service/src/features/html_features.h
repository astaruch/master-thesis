#pragma once

#ifndef PHISHSVC_FEATURE_HTML_FEATURES_H
#define PHISHSVC_FEATURE_HTML_FEATURES_H

#include "feature_base.h"

#include <string>
#include <unordered_map>

namespace feature {



class html_features {
public:
    html_features(std::string_view node_bin, std::string_view html_script,
        std::string_view url, uint64_t flags);
    void compute_values();

    const std::unordered_map<feature_enum::id, std::string_view> html_feature_column{
        { feature_enum::input_tag, "input_tag" }
    };

    const std::unordered_map<feature_enum::id, std::string_view> html_feature_arg{
        { feature_enum::input_tag, "--feat-input-tag" }
    };
private:
    std::string create_args();

    std::string_view _node_bin;
    std::string_view _html_script;
    std::string_view _url;

    std::string _cmd;
    uint64_t _feature_flags{0};

};

} // namespace feature

#endif // PHISHSVC_FEATURE_HTML_FEATURES_H