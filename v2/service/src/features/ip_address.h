#pragma once

#ifndef PHISHSVC_FEATURE_IP_ADDRESS_H
#define PHISHSVC_FEATURE_IP_ADDRESS_H

#include <string>

#include "feature_base.h"

class ip_address: public feature_base {
public:

    std::string name() override;
private:

};

#endif
