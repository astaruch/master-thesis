#pragma once

#ifndef PHISHSVC_TRAINING_DATA_H
#define PHISHSVC_TRAINING_DATA_H

#include <cstdint>
#include <string>
#include <vector>

class training_data {
public:
    /// Sets the feature flags
    void set_feature_flags(uint64_t flags);
    void set_url_feature_flags(uint64_t flags);
    /// Sets the html feature flags
    void set_html_feature_flags(uint64_t flags);

    /// Sets the source input data
    void set_input_data(std::vector<std::string> urls);

    /// Sets the label for the classification
    void set_label(double value);

    /// Sets the output name of the training data
    void set_output_name(std::string name);

    void set_node_bin(std::string_view node_bin);
    void set_html_script(std::string_view html_script);

    /// Method for generating the training data
    bool create_training_data();
private:
    /// Flags used to determine which features are enabled. See class \a feature_base
    uint64_t _feature_flags{0};
    uint64_t _url_feature_flags{0};
    uint64_t _html_feature_flags{0};

    /// Input data that will be transformed to training data
    std::vector<std::string> _urls;

    /// Label used for classification
    double _label;

    /// Name of the output filename
    std::string _output_name;

    std::string_view _node_bin;
    std::string_view _html_script;

    /// Training data containing feature
    std::vector<std::vector<double>> _training_data;

    /// Method to create csv header delimated by comma
    std::string create_csv_header();

    /// Method to transform input URLs to training data containing feature vectors
    std::vector<std::string> transform_urls_to_training_data();

    /// Compute feature vector for a given URL
    std::vector<double> compute_feature_vector(const std::string& url);
};


#endif
