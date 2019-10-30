#pragma once

#ifndef PHISHSVC_FEATURE_IP_ADDRESS_H
#define PHISHSVC_FEATURE_IP_ADDRESS_H

#include <string>

#include "../feature_base.h"

namespace feature {
class ip_address: public base {
public:
    /**
     * This feature is checking, wether host contains IP address in
     * any of following form:
     *  - normal dotted
     *  - octal
     *  - decimal
     *  - hexadecimal
     *
     * In case that IP address is found, feature returns 1. Otherwise 0.
     */
    double compute_value() override;
    std::string column_name() override
    {
        return "ip_address";
    }
private:

};

} // namespace feature

#endif // PHISHSVC_FEATURE_IP_ADDRESS_H
