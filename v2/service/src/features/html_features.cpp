#include "html_features.h"

#include "feature_base.h"

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

std::vector<std::string> html_features::get_output_from_program(const char* cmd)
{
    std::FILE* output_stream = popen(cmd, "r");
    if (output_stream == nullptr) {
        fmt::print(stderr, "There was an error executing command: {}\n", cmd);
        return {};
    }
    char line[256];
    std::vector<std::string> lines;
    while (fgets(line, 256, output_stream) != nullptr) {
        lines.push_back(std::string(line));
    }
    pclose(output_stream);
    return lines;
}

std::vector<double> html_features::compute_values()
{
    auto lines = get_output_from_program(_cmd.c_str());
    std::vector<double> f_vec;
    for (const auto& line: lines) {
        auto [column, value] = split_by_space(line);
        auto it = std::find_if(html_feature_column.begin(), html_feature_column.end(), [&](const auto& el) {
            return el.second == column;
        });
        if (it == html_feature_column.end()) {
            continue;
        }
        auto feature_id = it->first;
        auto computed_value = compute_value(feature_id, std::stoi(value));
        f_vec.push_back(computed_value);
    }
    return f_vec;
}

double html_features::compute_value(uint64_t fid, int value)
{
    switch (fid) {
    case feature_enum::input_tag:
        return compute_value_input_tag(value);
    }
    return 0;
}

double html_features::compute_value_input_tag(int value)
{
    return value > 0 ? 1 : 0;
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

