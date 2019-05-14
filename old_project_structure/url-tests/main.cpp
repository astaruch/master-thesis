#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>

#include <cxxopts.hpp>
#include <zconf.h>
#include "UrlTest.h"


int main(int argc, char **argv)
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
            ("a,address", "Port on where application will listen", cxxopts::value<std::string>())
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
            std::cout << "Starting app for one URL from commandline...\n";
            if (result.count("url"))
            {
                url_test.SetUrl(result["url"].as<std::string>());
            }
            else
            {
                std::cerr << "ERROR: Invalid URL...\n\n";
                return 1;
            }

            int value = url_test.PerformTests();

            std::cout << "Tests performed. Final weight: " << value << std::endl;
            return 0;
        }

        if (result.count("socket"))
        {
            std::string address;
            uint16_t nPort = 0;
            if (result.count("address"))
            {
                address = result["address"].as<std::string>();
                nPort = std::stoi(address);
            } else
            {
                std::cerr << "Please enter port for socket...\n";
                return 1;
            }
            std::cout << "Listening on localhost:" << nPort << "\n";
            int server_fd, new_socket;
            struct sockaddr_in server_address{};
            int opt = 1;
            int addrlen = sizeof(server_address);

            if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
            {
                std::cerr << "Socket failed\n";
                return -1;
            }

            if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                           &opt, sizeof(opt)))
            {
                std::cerr << "setsockopt failed\n";
                return -1;
            }

            server_address.sin_family = AF_INET;
            server_address.sin_addr.s_addr = INADDR_ANY;
            server_address.sin_port = htons(nPort);

            if (-1 == bind(server_fd, (struct sockaddr *)&server_address, sizeof(server_address)))
            {
                std::cout << "bind failed\n";
                return -1;
            }

            if (-1 == listen(server_fd, 3))
            {
                std::cerr << "listen failed\n";
                return -1;
            }

            if (-1 == (new_socket = accept(server_fd, (struct sockaddr *)&server_address, (socklen_t*)&addrlen)))
            {
                std::cerr << "accept failed\n";
                return -1;
            }
            char buffer[1024] = {0};
            while (0 < read(new_socket, buffer, 1024))
            {
                url_test.SetUrl(std::string(buffer));
                int value = url_test.PerformTests();
                std::cout << "Tests performed. Final weight: " << value << std::endl;
                std::string strBuffer = std::to_string(value);

                if (-1 == send(new_socket, strBuffer.c_str(), strBuffer.size(), 0))
                {
                    std::cerr << "send failed\n";
                    return -1;
                }
                memset(buffer, 0, 1024);
            }
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