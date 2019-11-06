#pragma once

#ifndef PHISHSVC_TRAINING_DATA_H
#define PHISHSVC_TRAINING_DATA_H

#include <cstdint>
#include <string>
#include <vector>

class training_data {
public:
    training_data() = default;
    training_data(bool verbose);
    void set_flags(uint64_t all, uint64_t url, uint64_t html, uint64_t host_based);

    void set_input_data(std::vector<std::string> urls);
    void set_label(int value);
    void set_output(std::FILE* file);

    void set_node_bin(std::string_view node_bin);
    void set_html_script(std::string_view html_script);

    /// Method for generating the training data
    bool create_training_data();
private:
    bool verbose_{false};
    /// Flags used to determine which features are enabled. See class \a feature_base
    uint64_t _feature_flags{0};
    uint64_t _url_feature_flags{0};
    uint64_t _html_feature_flags{0};
    uint64_t _host_based_feature_flags{0};

    /// Input data that will be transformed to training data
    std::vector<std::string> _urls;

    /// Label used for classification
    int _label;

    std::FILE* file_;

    std::string_view _node_bin;
    std::string_view _html_script;

    /// Training data containing feature
    std::vector<std::vector<double>> _training_data;

    /// Method to create csv header delimated by comma
    std::string create_csv_header();

    /// Method to transform input URLs to training data containing feature vectors
    std::vector<std::string> transform_urls_to_training_data();
};


#endif
