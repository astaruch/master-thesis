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
    training_data(const options& opts);

    void set_input_data(std::vector<std::string> urls);

    bool print_csv_training_data();
    std::vector<std::unordered_map<std::string_view, double>> get_data_for_model();
private:
    const options opts_;
    std::vector<std::string> _urls;
    std::string create_csv_header();
};

} // namespace phishscore


#endif
