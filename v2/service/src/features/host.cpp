#include "host.h"

#include <algorithm>
#include <regex>

namespace feature {

double gtld::compute_value()
{
    if (!_url_is_ok) {
        return _value;
    }
    const std::regex reg("(.com|.org|.net|.int|.edu|.gov|.mil|.cz|.sk)$");
    _value = std::regex_search(_url_obj.getHost(), reg) ? 0. : 1.;
    return _value;
}

double www_prefix::compute_value()
{
    if (!_url_is_ok) {
        return _value;
    }
    const std::regex reg("^(www-)");
    _value = std::regex_search(_url_obj.getHost(), reg) ? 1. : 0.;
    return _value;
}

double four_numbers::compute_value()
{
    if (!_url_is_ok) {
        return _value;
    }
    const std::regex reg("[[:digit:]]{4}");
    _value = std::regex_search(_url_obj.getHost(), reg) ? 1. : 0.;
    return _value;
}

double punycode::compute_value()
{
    if (!_url_is_ok) {
        return _value;
    }
    return _url_obj.getHost().find("xn--") == std::string::npos ? 0 : 1;
}

} // namespace feature

