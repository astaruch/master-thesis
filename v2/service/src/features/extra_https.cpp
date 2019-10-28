#include "extra_https.h"

#include <algorithm>

namespace feature {

double extra_https::compute_value()
{
    if (!_url_is_ok) {
        return _value;
    }
    auto host = _url_obj.getHost();
    std::transform(host.begin(), host.end(), host.begin(), ::tolower);

    if (host.find("https") == 0) {
        return 1;
    } else if (host.find("www.https") == 0) {
        return 1;
    }
    return 0;
}

std::string extra_https::name()
{
    return "extra_https";
}

} // namespace feature

