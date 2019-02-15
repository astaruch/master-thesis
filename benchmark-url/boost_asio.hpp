#ifndef BENCHMARK_URL_BOOST_ASIO_HPP
#define BENCHMARK_URL_BOOST_ASIO_HPP

#include <boost/asio.hpp>

class CBoostAsio {
public:
    explicit CBoostAsio(std::vector<std::string> urls): m_urls(std::move(urls)) {}

    void RunBenchmark();
    std::string Name() {
        return "Boost::Asio";
    }
private:
    std::vector<std::string> m_urls;
};

void CBoostAsio::RunBenchmark() {
    std::cout << "Size: " << m_urls.size() << std::endl;
}

#endif //BENCHMARK_URL_BOOST_ASIO_HPP
