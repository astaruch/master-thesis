#include "ip_address.h"

namespace feature {

ip_address::ip_address(): base()
{
}

double ip_address::compute_value()
{
    _value = -1.;
    return _value;
}

std::string ip_address::name()
{
    return "ip_address";
}

} // namespace feature
