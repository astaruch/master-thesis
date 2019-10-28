#pragma once

#ifndef PHISHSVC_FEATURE_NONSTD_PORT_H
#define PHISHSVC_FEATURE_NONSTD_PORT_H

#include <string>

#include "feature_base.h"

namespace feature {
class non_std_port: public base {
public:
    /**
     * This feature is checking wether URL contains user info
     */
    double compute_value() override;
    std::string column_name() override
    {
        return "non_std_port";
    }
};

} // namespace feature

#endif // PHISHSVC_FEATURE_NONSTD_PORT_H
