#pragma once

#ifndef PHISHSVC_HELP_FUNCTIONS_H
#define PHISHSVC_HELP_FUNCTIONS_H

#include <vector>
#include <string>

class help_functions {
public:
    static std::vector<std::string> get_output_from_program(const char* cmd);
    static std::vector<std::string> get_output_from_program(const std::string& cmd);
    static void print_output(const std::vector<std::string>& output);
    static double normalize_value(int min, int value, int max);
    static double normalize_value(int min, size_t value, int max);
    static double normalize_value(int min, long value, int max);
};


#endif // PHISHSVC_HELP_FUNCTIONS_H
