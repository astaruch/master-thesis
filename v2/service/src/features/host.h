#pragma once

#ifndef PHISHSVC_FEATURE_HOST_H
#define PHISHSVC_FEATURE_HOST_H

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


class www_prefix: public base {
public:
    double compute_value() override;
    std::string column_name() override
    {
        return "www_prefix";
    }
};

class four_numbers: public base {
public:
    double compute_value() override;
    std::string column_name() override
    {
        return "four_numbers";
    }
};

} // namespace feature

#endif // PHISHSVC_FEATURE_HOST_H
