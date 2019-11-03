#include "help_functions.h"

#include <fmt/format.h>

std::vector<std::string> help_functions::get_output_from_program(const char* cmd)
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

double help_functions::normalize_value(int min, int value, int max)
{
    value = std::max(min, std::min(value, max));
    return static_cast<double>(value - min) / static_cast<double>(max - min);
}

double help_functions::normalize_value(int min, size_t value, int max)
{
    return help_functions::normalize_value(min, static_cast<int>(value), max);
}
