#pragma once

#ifndef PHISHSVC_FEATURE_URL_LENGTH_H
#define PHISHSVC_FEATURE_URL_LENGTH_H

#include <string>

#include "feature_base.h"

namespace feature {
class url_length: public base {
public:
    /**
     * This feature is checking how long is a URL. We are using
     * the following formula to obtain normalized value:
     * v = ((url length) - (url min length)) / ((url max length) - (url min length))
     */
    double compute_value() override;
    static std::string name();
private:

    int _max_length{161};
    int _min_length{14};
};

} // namespace feature

#endif // PHISHSVC_FEATURE_URL_LENGTH_H
