#include "spec_char.h"

#include <algorithm>
namespace feature {

double spec_char_path::compute_value()
{
    if (!_url_is_ok) {
        return _value;
    }
    const auto& path = _url_obj.getPath();
    int count = static_cast<int>(std::count_if(path.begin(), path.end(), ::ispunct));
    return normalize_value(1, count, 8);
}

double spec_char_query::compute_value()
{
    if (!_url_is_ok) {
        return _value;
    }
    const auto& path = _url_obj.getQuery();
    int count = static_cast<int>(std::count_if(path.begin(), path.end(), ::ispunct));
    return normalize_value(0, count, 13);
}

double spec_char_fragment::compute_value()
{
    if (!_url_is_ok) {
        return _value;
    }
    const auto& path = _url_obj.getFragment();
    int count = static_cast<int>(std::count_if(path.begin(), path.end(), ::ispunct));
    return normalize_value(0, count, 5);
}

} // namespace feature

