#include "url_length.h"

namespace feature {

double url_length::compute_value()
{
    if (!_url_is_ok) {
        return _value;
    }
    auto len = _url.length() > _max_length ? _max_length : _url.length();
    return static_cast<double>(len - _min_length) / static_cast<double>(_max_length - _min_length);
}

std::string url_length::name()
{
    return "url_length";
}

} // namespace feature

