#include "html_features.h"

#include <fmt/format.h>

#include "feature_base.h"

namespace feature {

html_features::html_features(std::string_view node_bin,
                             std::string_view html_script,
                             std::string_view url,
                             uint64_t flags)
    : _node_bin(node_bin)
    , _html_script(html_script)
    , _url(url)
    , _feature_flags(flags)
{
    auto args = create_args();
    _cmd = fmt::format("{} {} --output-lines --url {} {}", _node_bin, _html_script, url, args);
    // fmt::print("{}\n", cmd);
}

void html_features::compute_values()
{

}

std::string html_features::create_args()
{
    std::string args;
    for (const auto id: feature_enum::html) {
        if (_feature_flags & id) {
            args += fmt::format("{} ", html_feature_arg.at(id));
        }
    }
    return args;
}

} // namespace feature

