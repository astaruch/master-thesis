#include "help_functions.h"

#include <ctime>
#include <chrono>

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
        auto str = std::string(line);
        if (str.back() == '\n') str.pop_back();
        if (str.back() == '\r') str.pop_back();
        lines.push_back(std::move(str));
    }
    pclose(output_stream);
    return lines;
}

std::vector<std::string> help_functions::get_output_from_program(const std::string& cmd)
{
    return get_output_from_program(cmd.c_str());
}

void help_functions::print_output(const std::vector<std::string>& output)
{
    for (const auto& line: output) {
        fmt::print("{}\n", line);
    }
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

double help_functions::normalize_value(int min, long value, int max)
{
    return help_functions::normalize_value(min, static_cast<int>(value), max);
}

double help_functions::normalize_value(long min, long value, long max)
{
    value = std::max(min, std::min(value, max));
    return static_cast<double>(value - min) / static_cast<double>(max - min);
}

double help_functions::normalize_date_string(std::string_view date)
{
    long timestamp_start = 0;
    long timestamp_end = 2147483647;
    struct tm tm;
    auto iso_data = fmt::format("{}-{}-{}", date.substr(0, 4), date.substr(4, 2), date.substr(6));
    // printf("%s\n", iso_data.c_str());
    strptime(iso_data.c_str(), "%Y-%m-%d", &tm);
    // If the std::tm object was obtained from std::get_time or the POSIX strptime, the value of
    // tm_isdst is indeterminate, and needs to be set explicitly before calling mktime.
    tm.tm_isdst = 0;
    tm.tm_hour = 0;
    tm.tm_min = 0;
    tm.tm_sec = 0;
    time_t t = mktime(&tm);
    // fmt::print("{} {} {}\n", timestamp_start, t, timestamp_end);
    return normalize_value(timestamp_start, std::move(t), timestamp_end);
}
