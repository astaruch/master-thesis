#pragma once

#ifndef PHISHSVC_FEATURE_QUERY_LENGTH_H
#define PHISHSVC_FEATURE_QUERY_LENGTH_H

#include <string>

#include "feature_base.h"

namespace feature {
class query_length: public base {
public:
    /**
     * This feature is checking how long is a QUERY. We are using
     * the following formula to obtain normalized value:
     * v = ((query length) - (query min length)) / ((query max length) - (query min length))
     */
    double compute_value() override;
    std::string column_name() override
    {
        return "query_length";
    }
private:

    size_t _min_length{6};
    size_t _max_length{220};
};

} // namespace feature

#endif // PHISHSVC_FEATURE_QUERY_LENGTH_H
