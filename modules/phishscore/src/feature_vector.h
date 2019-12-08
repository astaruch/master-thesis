
#pragma once

#ifndef PHISHSCORE_FEATURE_VECTOR
#define PHISHSCORE_FEATURE_VECTOR

#include "options.h"

namespace phishscore {

class feature_vector {
public:
    feature_vector(const options& opts, const std::string& url);

    void print_in_csv_format();
private:
    const options& opts_;
    std::string url_;

};

} // namespace phishscore

#endif // PHISHSCORE_FEATURE_VECTOR
