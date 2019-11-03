#pragma once

#ifndef PHISHSVC_FEATURE_PATH_LENGTH_H
#define PHISHSVC_FEATURE_PATH_LENGTH_H

#include <string>

#include "../feature_base.h"

namespace feature {
class path_length: public base {
public:
    /**
     * This feature is checking how long is a PATH. We are using
     * the following formula to obtain normalized value:
     * v = ((path length) - (path min length)) / ((path max length) - (path min length))
     */
    double compute_value() override;
private:

    size_t _min_length{1};
    size_t _max_length{89};
};

} // namespace feature

#endif // PHISHSVC_FEATURE_PATH_LENGTH_H
