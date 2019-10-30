#include "spec_keywords.h"

#include <algorithm>
#include <regex>

namespace feature {

double spec_keywords::compute_value()
{
    if (!_url_is_ok) {
        return _value;
    }
    std::regex reg("secure|account|webscr|login|ebayisapi|signin|banking|confirm", std::regex::icase | std::regex::optimize);
    std::smatch match;
    int count = 0;
    // static_cast<int>(match.size());
    auto url = _url;
    while (std::regex_search(url, match, reg)) {
        count++;
        url = match.suffix();
    }
    return normalize_value(0, count, 10);
}

} // namespace feature

