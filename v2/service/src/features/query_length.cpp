#include "query_length.h"

namespace feature {

double query_length::compute_value()
{
    if (!_url_is_ok) {
        return _value;
    }
    auto len = std::min(_url_obj.getQuery().length(), _max_length);
    len = std::max(len, _min_length);
    return static_cast<double>(len - _min_length) / static_cast<double>(_max_length - _min_length);
}

std::string query_length::name()
{
    return "query_length";
}

} // namespace feature

