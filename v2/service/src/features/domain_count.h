#pragma once

#ifndef PHISHSVC_FEATURE_DOMAIN_COUNT_H
#define PHISHSVC_FEATURE_DOMAIN_COUNT_H

#include <string>

#include "feature_base.h"

namespace feature {
class domain_count: public base {
public:
    /**
     * This feature is checking wether URL contains user info
     */
    double compute_value() override;
    static std::string name();
private:
    int _min_count{1};
    int _max_count{21};
};

} // namespace feature

#endif // PHISHSVC_FEATURE_DOMAIN_COUNT_H
