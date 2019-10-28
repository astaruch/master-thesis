#include "https_used.h"

#include <algorithm>

namespace feature {

double https_used::compute_value()
{
    if (!_url_is_ok) {
        return _value;
    }
    auto scheme = _url_obj.getScheme();
    std::transform(scheme.begin(), scheme.end(), scheme.begin(), tolower);
    return scheme == "https" ? 0 : 1;
}

} // namespace feature

