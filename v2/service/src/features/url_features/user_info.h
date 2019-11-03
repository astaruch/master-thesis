#pragma once

#ifndef PHISHSVC_FEATURE_USER_INFO_H
#define PHISHSVC_FEATURE_USER_INFO_H

#include <string>

#include "../feature_base.h"

namespace feature {
class user_info: public base {
public:
    /**
     * This feature is checking wether URL contains user info
     */
    double compute_value() override;
};

} // namespace feature

#endif // PHISHSVC_FEATURE_USER_INFO_H
