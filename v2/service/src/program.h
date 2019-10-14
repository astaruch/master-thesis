#pragma once

#ifndef PHISHSVC_PROGRAM_H
#define PHISHSVC_PROGRAM_H

#include <cxxopts.hpp>

class program {
public:
    program(int argc, char** argv);

    /// This method will perform general operations such as checking help
    void check_general_opts();
private:
    bool _help;
    bool _version;
    cxxopts::Options _options;
    // cxxopts::ParseResult _result;
};

#endif // PHISHSVC_PROGRAM_H
