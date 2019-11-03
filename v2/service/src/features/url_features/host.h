#pragma once

#ifndef PHISHSVC_FEATURE_HOST_H
#define PHISHSVC_FEATURE_HOST_H

#include <string>

#include "../feature_base.h"

namespace feature {
class gtld: public base {
public:
    double compute_value() override;
};


class www_prefix: public base {
public:
    double compute_value() override;
};

class four_numbers: public base {
public:
    double compute_value() override;
};

class punycode: public base {
public:
    double compute_value() override;
};

} // namespace feature

#endif // PHISHSVC_FEATURE_HOST_H
