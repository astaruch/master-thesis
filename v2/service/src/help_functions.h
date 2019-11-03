#pragma once

#ifndef PHISHSVC_HELP_FUNCTIONS_H
#define PHISHSVC_HELP_FUNCTIONS_H

#include <vector>
#include <string>

class help_functions {
public:
    static std::vector<std::string> get_output_from_program(const char* cmd);
};


#endif // PHISHSVC_HELP_FUNCTIONS_H
