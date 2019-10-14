#include "program.h"

#include <cxxopts.hpp>
#include <spdlog/spdlog.h>

program::program(int argc, char** argv)
    : _options("phishsvc", "Application for phishing defence")
    , _help(false)
    , _version(false)
{
    _options
        .positional_help("[positional args]")
        .show_positional_help();

    _options.add_options()
        ("h,help", "Prints help and exit", cxxopts::value<bool>(_help))
        ("v,version", "Prints version information and exit", cxxopts::value<bool>(_version))
    ;

    auto _result = _options.parse(argc, argv);
}

void program::check_general_opts()
{
    if (_help) {
        std::cout << _options.help({""}) << "\n";
        exit(0);
    }

    if (_version) {
        std::cout << "0.0.1" << "\n";
        exit(0);
    }
}
