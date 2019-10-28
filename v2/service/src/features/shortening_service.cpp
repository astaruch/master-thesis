#include "shortening_service.h"

#include <algorithm>
#include <regex>

namespace feature {

double shortening_service::compute_value()
{
    if (!_url_is_ok) {
        return _value;
    }
    const std::regex reg("(bitly.|bit.ly|tinyurl.com|goo.gl|tiny.cc|livechatinc.com|is.gd|soo.gd|clicky.me|cutt.ly)");
    _value = std::regex_search(_url_obj.getHost(), reg) ? 1. : 0.;
    return _value;
}

std::string shortening_service::name()
{
    return "shortening_service";
}

} // namespace feature

