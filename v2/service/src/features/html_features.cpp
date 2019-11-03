#include "html_features.h"

#include "feature_base.h"
#include "../help_functions.h"

#include <cstdio>
#include <fmt/format.h>


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

std::tuple<std::string, std::string> html_features::split_by_space(const std::string& str)
{
    auto found = str.find(' ');
    return {str.substr(0, found), str.substr(found + 1)};
}

std::vector<double> html_features::compute_values()
{
    auto lines = help_functions::get_output_from_program(_cmd.c_str());
    std::vector<double> f_vec;
    for (const auto& line: lines) {
        auto [column, value] = split_by_space(line);
        f_vec.push_back(std::stod(value));
    }
    return f_vec;
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

void html_features::set_flags(uint64_t flags)
{
    _feature_flags = flags;
}

void html_features::set_url(std::string_view url)
{
    _url = url;
}

} // namespace feature

