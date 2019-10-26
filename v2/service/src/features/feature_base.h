#pragma once

#ifndef PHISHSVC_BASE_FEATURE_H
#define PHISHSVC_BASE_FEATURE_H

enum feature_name: uint64_t {
    ip_address = 1,
};

class feature_base {
public:

    virtual std::string name() = 0;
private:
};
#endif
