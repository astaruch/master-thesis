#pragma once

#ifndef PHISHSVC_TRAINING_DATA_H
#define PHISHSVC_TRAINING_DATA_H

#include <cstdint>
#include <string>
#include <vector>

class training_data {
public:

    void set_feature_flags(uint64_t flags);
    void set_input_data(std::vector<std::string> urls);
    void set_label(double value);
    void set_output_name(std::string name);
    bool create_training_data();
private:
    uint64_t _feature_flags;
    std::vector<std::string> _urls;
    double _label;
    std::string _output_name;

    std::string create_csv_header();
};


#endif
