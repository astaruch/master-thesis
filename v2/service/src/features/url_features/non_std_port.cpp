#include "non_std_port.h"

namespace feature {

double non_std_port::compute_value()
{
    if (!_url_is_ok) {
        return _value;
    }
    const auto& port = _url_obj.getPort();
    return (port != 80 && port != 443) ? 1 : 0;
}

} // namespace feature

