#pragma once

#ifndef PHISHSVC_FEATURE_SPEC_CHAR_H
#define PHISHSVC_FEATURE_SPEC_CHAR_H

#include <string>

#include "../feature_base.h"

namespace feature {
class spec_char_path: public base {
public:
    double compute_value() override;
};

class spec_char_query: public base {
public:
    double compute_value() override;
};

class spec_char_fragment: public base {
public:
    double compute_value() override;
};

class spec_char_host: public base {
public:
    double compute_value() override;
};

} // namespace feature

#endif // PHISHSVC_FEATURE_SPEC_CHAR_H
