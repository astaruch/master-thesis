#include <chrono>
#include <iostream>
#include <fstream>
#include <thread>

#include "SkyrUrl.hpp"
#include "Uriparser.hpp"

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
    libraries.push_back(std::make_unique<SkyrUrl>(urls));
    libraries.push_back(std::make_unique<Uriparser>(urls));

    for (auto& library: libraries) {
        auto start = std::chrono::high_resolution_clock::now();
        library->DoBenchmark();
        auto end = std::chrono::high_resolution_clock::now();

        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end-start).count();
        std::cout << library->Name() << " benchmark. " <<
            "Parsed: " << library->ParsedSize() << ". " <<
            "Invalid: " << library->InvalidSize() << ". " <<
            "Elapsed: " << elapsed << "us.\n";
    }

    return 0;
}