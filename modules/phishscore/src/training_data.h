#pragma once

#ifndef PHISHSVC_TRAINING_DATA_H
#define PHISHSVC_TRAINING_DATA_H

#include "options.h"

#include <cstdint>
#include <string>
#include <vector>
#include <unordered_map>

namespace phishscore {

class training_data {
public:
    training_data() = default;
    training_data(bool verbose, bool output_include_url, bool output_extra_values);
    training_data(const options& opts);
    void set_flags(uint64_t all, uint64_t url, uint64_t html, uint64_t host_based);

    void set_input_data(std::vector<std::string> urls);
    void set_label(int value);
    void set_output(std::FILE* file);

    void set_html_features_opts(std::string_view htmlfeatures_exe, uint16_t port = 0);

    bool print_csv_training_data();
    std::vector<std::unordered_map<std::string_view, double>> get_data_for_model();
private:
    const options opts_;
    bool verbose_{false};
    bool output_include_url_{false};
    bool output_extra_values_{false};
    /// Flags used to determine which features are enabled. See class \a feature_base
    uint64_t _feature_flags{0};
    uint64_t _url_feature_flags{0};
    uint64_t _html_feature_flags{0};
    uint64_t _host_based_feature_flags{0};
    uint16_t port_{0};

    /// Input data that will be transformed to training data
    std::vector<std::string> _urls;

    /// Label used for classification
    int _label;

    std::FILE* file_;

    std::string_view htmlfeatures_bin_;

    /// Method to create csv header delimated by comma
    std::string create_csv_header();

};

} // namespace phishscore


#endif
