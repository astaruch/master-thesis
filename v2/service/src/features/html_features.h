#pragma once

#ifndef PHISHSVC_FEATURE_HTML_FEATURES_H
#define PHISHSVC_FEATURE_HTML_FEATURES_H

#include "features.h"
#include "feature_enum.h"

#include <tuple>
#include <string>
#include <unordered_map>

class html_features_t {
public:
    html_features_t(std::string_view url, uint64_t flags, std::string_view node_bin,
        std::string_view html_script);

    std::vector<double> compute_values();

    const std::unordered_map<feature_enum::id, std::string_view> html_feature_arg{
        { feature_enum::input_tag, "--feat-input-tag" },
        { feature_enum::src_link, "--feat-src-link" },
        { feature_enum::form_handler, "--feat-form-handler" },
        { feature_enum::invisible_iframe, "--feat-invisible-iframe" },
        { feature_enum::rewrite_statusbar, "--feat-rewrite-statusbar" },
        { feature_enum::disable_rightclick, "--feat-disable-rightclick" },
        { feature_enum::ahref_link, "--feat-ahref-link" },
        { feature_enum::popup_window, "--feat-popup-window" },
        { feature_enum::favicon_link, "--feat-favicon-link" },
        { feature_enum::old_technologies, "--feat-old-technologies" },
        { feature_enum::missleading_link, "--feat-missleading-link" },
        { feature_enum::hostname_title, "--feat-hostname-title" },
    };

private:
    std::string create_args();
    std::tuple<std::string, std::string> split_by_space(const std::string& str);

    std::string_view _url;
    uint64_t _feature_flags{0};
    std::string_view _node_bin;
    std::string_view _html_script;

    std::string _cmd;

};

#endif // PHISHSVC_FEATURE_HTML_FEATURES_H
