#include <chrono>
#include <iostream>
#include <fstream>
#include <thread>

#include "SkyrUrl.hpp"
//#include "boost_asio.hpp"

using namespace std::chrono_literals;

int main()
{

    constexpr const char* filename = "../urls-5000.txt";
    std::ifstream file(filename);
    std::string url;
    std::vector<std::string> urls;
    while (std::getline(file, url)) {
        urls.push_back(url);
    }
    auto url_library = SkyrUrl(std::move(urls));

    auto start = std::chrono::high_resolution_clock::now();
    url_library.DoBenchmark();
    auto end = std::chrono::high_resolution_clock::now();

    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();

    std::cout << url_library.Name() << " benchmark. Elapsed: " << elapsed << "ms.\n";

    return 0;
}