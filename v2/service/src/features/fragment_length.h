#pragma once

#ifndef PHISHSVC_FEATURE_FRAGMENT_LENGTH_H
#define PHISHSVC_FEATURE_FRAGMENT_LENGTH_H

#include <string>

#include "feature_base.h"

namespace feature {
class fragment_length: public base {
public:
    /**
     * This feature is checking how long is a FRAGMENT. We are using
     * the following formula to obtain normalized value:
     * v = ((fragment length) - (fragment min length)) / ((fragment max length) - (fragment min length))
     */
    double compute_value() override;
    std::string column_name() override
    {
        return "fragment_length";
    }
private:

    size_t _min_length{1};
    size_t _max_length{155};
};

} // namespace feature

#endif // PHISHSVC_FEATURE_FRAGMENT_LENGTH_H
