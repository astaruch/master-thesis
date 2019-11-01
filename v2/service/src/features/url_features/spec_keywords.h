#pragma once

#ifndef PHISHSVC_FEATURE_SPEC_KEYWORDS_H
#define PHISHSVC_FEATURE_SPEC_KEYWORDS_H

#include <string>

#include "../feature_base.h"

namespace feature {
class spec_keywords: public base {
public:
    double compute_value() override;
    std::string column_name() override
    {
        return "spec_keywords";
    }
};

} // namespace feature

#endif // PHISHSVC_FEATURE_SPEC_KEYWORDS_H