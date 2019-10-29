#pragma once

#ifndef PHISHSVC_FEATURE_TLD_H
#define PHISHSVC_FEATURE_TLD_H

#include <string>

#include "feature_base.h"

namespace feature {
class gtld: public base {
public:
    double compute_value() override;
    std::string column_name() override
    {
        return "gtld";
    }
};

} // namespace feature

#endif // PHISHSVC_FEATURE_TLD_H
