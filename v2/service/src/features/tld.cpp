#include "tld.h"

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

} // namespace feature

