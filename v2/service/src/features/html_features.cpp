#include "html_features.h"

#include "../help_functions.h"

#include <sstream>
#include <fmt/format.h>

html_features_t::html_features_t(std::string_view url,
                                 uint64_t flags,
                                 std::string_view node_bin,
                                 std::string_view html_script)
    : url_(url)
    , flags_(flags)
    , node_bin_(node_bin)
    , html_script_(html_script)
{
    auto args = create_args();
    cmd_ = fmt::format("{} {} --output-lines --url '{}' {}", node_bin_, html_script_, url, args);
}

html_features_t::html_features_t(std::string_view url,
                                 uint64_t flags,
                                 std::string_view exe_path)
    : url_(url)
    , flags_(flags)
    , exe_path_(exe_path)
{
    auto args = create_args();
    cmd_ = fmt::format("{} {} --output-values-string --url '{}'", exe_path_, args, url);
}

std::tuple<std::string, std::string> html_features_t::split_by_space(const std::string& str)
{
    auto found = str.find(' ');
    return {str.substr(0, found), str.substr(found + 1)};
}

std::vector<double> html_features_t::compute_values()
{
    auto lines = help_functions::get_output_from_program(cmd_.c_str());
    std::vector<double> f_vec;
    for (const auto& line: lines) {
        auto [column, value] = split_by_space(line);
        f_vec.push_back(std::stod(value));
    }
    return f_vec;
}

std::vector<double> html_features_t::get_values_from_external_script()
{
    // 0th line are values
    // 1th line is optional error message
    auto output = help_functions::get_line_from_program_if_exists(cmd_.c_str(), 0);
    std::vector<double> result;
    std::stringstream stream(output);
    std::string value_str;
    while(std::getline(stream, value_str, ',')) {
        result.push_back(std::stod(value_str));
    }
    return result;
}

std::string html_features_t::create_args()
{
    std::string args;
    for (const auto id: feature_enum::html) {
        if (flags_ & id) {
            args += fmt::format("{} ", html_feature_arg.at(id));
        }
    }
    return args;
}
