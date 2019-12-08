#pragma once

#ifndef PHISHSVC_HELP_FUNCTIONS_H
#define PHISHSVC_HELP_FUNCTIONS_H

#include <vector>
#include <string>
#include <string_view>

class help_functions {
public:
    static std::string get_output_from_program_in_string(const char* cmd);
    // execute a system command and return output in vector of strings without endlines (\r\n are trimmd)
    static std::vector<std::string> get_output_from_program(const char* cmd);
    static std::vector<std::string> get_output_from_program(const std::string& cmd);
    // returns the n-th line from output. if output has lesser lines, empty string is returned
    static std::string get_line_from_program_if_exists(const char* cmd, size_t pos);
    static std::string get_line_from_program_if_exists(const std::string& cmd, size_t pos);
    static void print_output(const std::vector<std::string>& output);
    static double normalize_value(int min, int value, int max);
    static double normalize_value(int min, size_t value, int max);
    static double normalize_value(int min, long value, int max);
    static double normalize_value(long min, long value, long max);
    static double normalize_date_string(std::string_view date);
    static double normalize_iso_date_string(std::string_view date);
    static std::vector<std::string> str2vec(std::string_view str);
};


#endif // PHISHSVC_HELP_FUNCTIONS_H
