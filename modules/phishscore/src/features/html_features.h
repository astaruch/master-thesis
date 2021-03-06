#pragma once

#ifndef PHISHSVC_FEATURE_HTML_FEATURES_H
#define PHISHSVC_FEATURE_HTML_FEATURES_H

#include "feature_enum.h"

#include "../options.h"

#include <tuple>
#include <string>
#include <unordered_map>
#include <vector>

#include <nlohmann/json.hpp>

namespace phishscore {

class html_features_t {
public:
    html_features_t(std::string_view url, const options& opts);
    html_features_t(std::string_view url, uint64_t flags, std::string_view exe_path,
        bool extra_values);

    std::vector<double> compute_values();
    std::vector<double> get_values_from_external_script();
    std::string get_header();
    std::unordered_map<std::string_view, double> compute_values_map() const;


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
    nlohmann::json prepare_request() const;
    nlohmann::json get_response_from_html_analysis(const std::string& request) const;

    std::string_view url_;
    const options opts_;
    uint64_t flags_{0};
    std::string_view exe_path_;
    uint16_t port_;
    std::string cmd_;

};

} // namespace phishscore
#endif // PHISHSVC_FEATURE_HTML_FEATURES_H
