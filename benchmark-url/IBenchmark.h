#include <utility>

#ifndef BENCHMARK_URL_IBENCHMARK_H
#define BENCHMARK_URL_IBENCHMARK_H

#include <string>
#include <vector>

class IBenchmark {
public:
    explicit IBenchmark(std::vector<std::string> urls) : m_raw_urls(std::move(urls)) {}

    virtual void DoBenchmark() = 0;

    virtual std::string Name() = 0;

    size_t InvalidSize() {
        return m_invalid_urls.size();
    }

    virtual size_t ParsedSize() = 0;

    virtual size_t BytesSize() = 0;

protected:
    std::vector<std::string> m_raw_urls;
    std::vector<std::string> m_invalid_urls;

};

#endif //BENCHMARK_URL_IBENCHMARK_H
