#include <chrono>
#include <iostream>
#include <fstream>
#include <thread>

#include "CNetworkUri.hpp"
#include "CSkyrUrl.hpp"
#include "CUriparser.hpp"

using namespace std::chrono_literals;

int main(int argc, char** argv)
{

    constexpr const char* filename = "../urls-5000.txt";
    std::ifstream file(filename);
    std::string url;
    std::vector<std::string> urls;
    while (std::getline(file, url)) {
        urls.push_back(url);
    }

    std::vector<std::unique_ptr<IBenchmark>> libraries;
    libraries.push_back(std::make_unique<CSkyrUrl>(urls));
    libraries.push_back(std::make_unique<CUriparser>(urls));
    libraries.push_back(std::make_unique<CNetworkUri>(urls));

    std::cout << "Parsed\t|Invalid\t|Elapsed (ms)\n";
    for (auto& library: libraries) {
        auto start = std::chrono::high_resolution_clock::now();
        library->DoBenchmark();
        auto end = std::chrono::high_resolution_clock::now();

        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
        std::cout << library->Name() << "\n" << library->ParsedSize() << "\t|" <<
            library->InvalidSize() << "\t|" << elapsed << "\n\n";
    }

    return 0;
}