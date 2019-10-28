#include "domain_count.h"

#include <algorithm>

namespace feature {

double domain_count::compute_value()
{
    if (!_url_is_ok) {
        return _value;
    }
    int domains = static_cast<int>(std::count(_url_obj.getHost().begin(),  _url_obj.getHost().end(), '.'));
    int count = std::min(domains, _max_count);
    count = std::max(count, _min_count);
    return static_cast<double>(count - _min_count) / static_cast<double>(_max_count - _min_count);
}

} // namespace feature

