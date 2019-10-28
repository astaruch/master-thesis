#include "user_info.h"

namespace feature {

double user_info::compute_value()
{
    if (!_url_is_ok) {
        return _value;
    }
    return _url_obj.getUserInfo().length() == 0 ? 0 : 1;
}

std::string user_info::name()
{
    return "user_info";
}

} // namespace feature

