#pragma once

#ifndef PHISHSVC_FEATURE_IP_ADDRESS_H
#define PHISHSVC_FEATURE_IP_ADDRESS_H

#include <string>

#include "feature_base.h"

namespace feature {
class ip_address: public base {
public:
    double compute_value() override;
    static std::string name();
private:

};

} // namespace feature

#endif // PHISHSVC_FEATURE_IP_ADDRESS_H
