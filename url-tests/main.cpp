#include <iostream>
#include <cxxopts.hpp>

#include "UrlTest.h"

int
main(int argc, char **argv)
{
    try
    {
        cxxopts::Options options("url_test", " - perform test on a given URL");
        options
            .positional_help("[optional args]")
            .show_positional_help();


        const std::string def_length = "53";
        const std::string def_depth = "5";
        const std::string def_keywords = "webscr,secure,banking,ebayisapi,account,confirm,login,signin";

        options.add_options("General")
            ("h,help", "Print help")
            ("u,url", "URL", cxxopts::value<std::string>()->default_value(""))
            ("cmdline", "Whether application will take URL input from a commandline")
            ("socket", "Whether application will listen for URLs on socket")
            ("icap", "Whether application will work as ICAP server")
            ("rest", "Whether application will communicate through REST API")
            ("a,address", "Address where application will listen", cxxopts::value<std::string>())
                ;

        options.add_options("Test")
            ("test-all", "Perform all tests with")
            ("test-length", "Test whether URL exceeds given length",
             cxxopts::value<int>()->implicit_value("53")->default_value("53"))
            ("test-depth", "Test whether URL exceeds given depth",
             cxxopts::value<int>()->implicit_value("5")->default_value("5"))
            ("test-special-chars", "Test whether URL has some special characters")
            ("test-keywords", "Test whether URL contains any from given keywords separated by comma",
             cxxopts::value<std::string>()->implicit_value(def_keywords)->default_value(""))
            ("test-encoded-url", "Test whether URL contains encoded characters")
            ("test-ip-address-occurrence", "Test whether hostname contains IP address")
            ("test-non-standard-port", "Test whether URL port is standard (http=80/https=443")
            ("test-non-standard-tld", "Test whether URL has suspicious TLD")
            ("test-scripts-in-query", "Test whether query contains scripts and is XSS-prone")
            ("test-chars-frequency", "Tests characters frequency in a domain name")
                ;


        auto result = options.parse(argc, argv);

        if (result.count("help") || result.count("h"))
        {
            std::cout << options.help({"General", "Test"}) << std::endl;
            return 0;
        }

        UrlTest url_test;
        if (result.count("test-all"))
        {
            url_test.AddTestLength(result["test-length"].as<int>());
            url_test.AddTestDepth(result["test-depth"].as<int>());
            url_test.AddTestSpecialChars();
            url_test.AddTestKeywords(result["test-keywords"].as<std::string>());
            url_test.AddTestEncodedChars();
            url_test.AddTestIpAddressOccurrence();
            url_test.AddTestNonStandardPort();
            url_test.AddTestNonStandardTLD();
            url_test.AddTestScriptsInQuery();
            url_test.AddTestCharsFrequency();
        }
        else
        {
            if (result.count("test-length"))
            {
                url_test.AddTestLength(result["test-length"].as<int>());
            }
            if (result.count("test-depth"))
            {
                url_test.AddTestDepth(result["test-depth"].as<int>());
            }
            if (result.count("test-special-chars"))
            {
                url_test.AddTestSpecialChars();
            }
            if (result.count("test-keywords"))
            {
                url_test.AddTestKeywords(result["test-keywords"].as<std::string>());
            }
            if (result.count("test-encoded-chars"))
            {
                url_test.AddTestEncodedChars();
            }
            if (result.count("test-ip-address-occurrence"))
            {
                url_test.AddTestIpAddressOccurrence();
            }
            if (result.count("test-non-standard-port"))
            {
                url_test.AddTestNonStandardPort();
            }
            if (result.count("test-non-standard-tld"))
            {
                url_test.AddTestNonStandardTLD();
            }
            if (result.count("test-scripts-in-query"))
            {
                url_test.AddTestScriptsInQuery();
            }
            if (result.count("test-chars-frequency"))
            {
                url_test.AddTestCharsFrequency();
            }
        }

        if (result.count("cmdline"))
        {
            std::cout << "Starting app for one URL from commandline\n";
            if (result.count("url"))
            {
                url_test.SetUrl(result["url"].as<std::string>());
            }
            else
            {
                std::cerr << "Invalid URL\n";
                std::cout << options.help({"General"}) << std::endl;
                return 1;
            }

            int value = url_test.PerformTests();

            std::cout << "Tests performed. Final weight: " << value << std::endl;
            return 0;
        }

        if (result.count("socket"))
        {
            std::string address;
            if (result.count("address"))
            {
                address = result["address"].as<std::string>();
            } else
            {
                std::cerr << "Please enter address for socket...\n";
                std::cerr << options.help({"General"}) << std::endl;
                return 1;
            }
            std::cout << "Listening on address '" << address << "'...\n";
        }

        if (result.count("icap"))
        {
            std::cout << "Not implemented yet...\n";
            return 0;
        }




    }
    catch (const cxxopts::OptionException &e)
    {
        std::cout << "error parsing options: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}