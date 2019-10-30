#pragma once

#ifndef PHISHSVC_ALL_FEATURES_H
#define PHISHSVC_ALL_FEATURES_H

#include <memory>

#include "feature_base.h"
#include "url_features/url_features.h"

namespace feature {

class creator {
public:
    static std::unique_ptr<feature::base> create_feature_from_flag(feature_enum::id id);
};

}
#endif // PHISHSVC_ALL_FEATURES_H
