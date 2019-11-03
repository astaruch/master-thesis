#pragma once

#ifndef PHISHSVC_FEATURE_HTTPS_USED_H
#define PHISHSVC_FEATURE_HTTPS_USED_H

#include <string>

#include "../feature_base.h"

namespace feature {
class https_used: public base {
public:
    /**
     * This feature is checking wether URL is usingi https
     */
    double compute_value() override;
};

} // namespace feature

#endif // PHISHSVC_FEATURE_HTTPS_USED_H
