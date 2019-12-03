#include "model_checker.h"

#include "help_functions.h"

#include <fmt/format.h>

model_checker_t::model_checker_t(std::string_view path)
    : path_(path)
{
}

double model_checker_t::predict(const std::string& unescaped_json)
{
    auto cmd = fmt::format("{} --data-json \"{}\"", path_, unescaped_json);
    auto phishingness = help_functions::get_output_from_program_in_string(cmd.c_str());
    return std::stod(phishingness);
}
