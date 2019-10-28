#pragma once

#ifndef PHISHSVC_FEATURE_HOST_LENGTH_H
#define PHISHSVC_FEATURE_HOST_LENGTH_H

#include <string>

#include "feature_base.h"

namespace feature {
class host_length: public base {
public:
    /**
     * This feature is checking how long is a HOST. We are using
     * the following formula to obtain normalized value:
     * v = ((host length) - (host min length)) / ((host max length) - (host min length))
     */
    double compute_value() override;
    static std::string name();
private:

    size_t _min_length{10};
    size_t _max_length{37};
};

} // namespace feature

#endif // PHISHSVC_FEATURE_HOST_LENGTH_H
