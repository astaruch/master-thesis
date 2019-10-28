#include "path_length.h"

namespace feature {

double path_length::compute_value()
{
    if (!_url_is_ok) {
        return _value;
    }
    auto len = _url_obj.getPath().length() > _max_length ? _max_length : _url_obj.getPath().length();
    return static_cast<double>(len - _min_length) / static_cast<double>(_max_length - _min_length);
}

std::string path_length::name()
{
    return "path_length";
}

} // namespace feature

