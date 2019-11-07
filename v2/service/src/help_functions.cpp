#include "help_functions.h"

#include <ctime>
#include <chrono>

#include <fmt/format.h>

std::vector<std::string> help_functions::get_output_from_program(const char* cmd)
{
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        fmt::print(stderr, "There was an error executing command: {}\n", cmd);
        throw std::runtime_error("popen() failed!");
    }
    std::array<char, 256> line;
    std::vector<std::string> lines;
    while (fgets(line.data(), 256, pipe.get()) != nullptr) {
        auto str = std::string(line.data());
        if (str.back() == '\n') str.pop_back();
        if (str.back() == '\r') str.pop_back();
        lines.push_back(std::move(str));
    }
    return lines;
}

std::vector<std::string> help_functions::get_output_from_program(const std::string& cmd)
{
    return get_output_from_program(cmd.c_str());
}

std::string help_functions::get_line_from_program_if_exists(const char* cmd, size_t pos)
{
    auto output = get_output_from_program(cmd);
    if (output.size() > pos) {
        return output[pos];
    }
    return "";
}

std::string help_functions::get_line_from_program_if_exists(const std::string& cmd, size_t pos)
{
    return get_line_from_program_if_exists(cmd.c_str(), pos);
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
