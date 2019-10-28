#include "host_length.h"

namespace feature {

double host_length::compute_value()
{
    if (!_url_is_ok) {
        return _value;
    }
    auto len = std::min(_url_obj.getHost().length(), _max_length);
    len = std::max(len, _min_length);
    return static_cast<double>(len - _min_length) / static_cast<double>(_max_length - _min_length);
}

std::string host_length::name()
{
    return "host_length";
}

} // namespace feature

