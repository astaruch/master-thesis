#include "model_checker.h"

#include "help_functions.h"

#include <fmt/format.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

model_checker_t::model_checker_t(std::string_view path)
    : path_(path)
{
}

json model_checker_t::predict(const std::string& unescaped_json)
{
    auto cmd = fmt::format("{} --data-json \"{}\"", path_, unescaped_json);
    auto str = help_functions::get_output_from_program_in_string(cmd.c_str());
    return json::parse(str);
}
