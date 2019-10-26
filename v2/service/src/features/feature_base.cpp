#include "feature_base.h"

namespace feature {

base::base(): _url(std::string()), _value(-1)
{
}

void base::set_url(const std::string& url)
{
    _url = url;
}

} // namespace feature

