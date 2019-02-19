#include <iostream>
#include <cxxopts.hpp>

#include "UrlTest.h"


int main(int argc, char** argv)
{
    try {
        cxxopts::Options options("url_test", " - perform test on a given URL");
        options
                .positional_help("[optional args]")
                .show_positional_help();

        options.add_options()
                ("h,help", "Print help")
                ("u,url", "URL", cxxopts::value<std::string>()->default_value(""));

        options.add_options("Test")
                ("test-all", "Perform all tests with")
                ("test-length", "Test whether URL exceeds given length", cxxopts::value<int>()->implicit_value("53")->default_value("53"))
                ("test-depth", "Test whether URL exceeds given depth", cxxopts::value<int>()->implicit_value("5")->default_value("5"))
                ;

        auto result = options.parse(argc, argv);

        if (result.count("help") || result.count("h")) {
            std::cout << options.help({"", "Test"}) << std::endl;
            return 0;
        }

        UrlTest url_test;

        if (result.count("url")) {
            url_test.SetUrl(result["url"].as<std::string>());
        } else {
            std::cerr << "Invalid URL\n";
            std::cout << options.help({"", "Test"}) << std::endl;
            return 1;
        }

        if (result.count("test-all")) {
            url_test.AddTestLength(result["test-length"].as<int>());
            url_test.AddTestDepth(result["test-depth"].as<int>());
        }
        else {
            if (result.count("test-length")) {
                url_test.AddTestLength(result["test-length"].as<int>());
            }
            if (result.count("test-depth")) {
                url_test.AddTestDepth(result["test-depth"].as<int>());
            }
        }

        int value = url_test.PerformtTests();

        std::cout << "Tests performed. Final weight: " << value << std::endl;
    }
    catch (const cxxopts::OptionException& e) {
        std::cout << "error parsing options: " << e.what() << std::endl;
        return 1;
    }


    return 0;
}