#pragma once

#ifndef PHISHSVC_FEATURE_EXTRA_HTTPS_H
#define PHISHSVC_FEATURE_EXTRA_HTTPS_H

#include <string>

#include "feature_base.h"

namespace feature {
class extra_https: public base {
public:
    /**
     * This feature is checking wether host contains extra https token
     */
    double compute_value() override;
    static std::string name();
};

} // namespace feature

#endif // PHISHSVC_FEATURE_EXTRA_HTTPS_H
