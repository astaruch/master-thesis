#pragma once

#ifndef PHISHSVC_ALL_FEATURES_H
#define PHISHSVC_ALL_FEATURES_H

#include <memory>

#include "feature_base.h"
#include "ip_address.h"
#include "url_length.h"
#include "host_length.h"
#include "path_length.h"
#include "query_length.h"
#include "fragment_length.h"
#include "user_info.h"
#include "domain_count.h"
#include "https_used.h"
#include "extra_https.h"
#include "shortening_service.h"
#include "non_std_port.h"
#include "spec_char.h"

namespace feature {

class creator {
public:
    static std::unique_ptr<feature::base> create_feature_from_flag(feature_enum::id id);
};

}
#endif // PHISHSVC_ALL_FEATURES_H
