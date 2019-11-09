#ifndef BENCHMARK_URL_CPPRESTSDK_HPP
#define BENCHMARK_URL_CPPRESTSDK_HPP

#include <cpprest/base_uri.h>

#include "IBenchmark.h"

class CCppRestSdk : public IBenchmark {
public:
    explicit CCppRestSdk (std::vector<std::string> urls)
            : IBenchmark(std::move(urls)) {}

    void DoBenchmark() override;

    std::string Name() override;

    size_t ParsedSize() override { return m_parsed_urls.size(); }

    size_t BytesSize() override { return sizeof(std::vector<web::uri>) + (sizeof(web::uri) * m_parsed_urls.size()); }

private:
    std::vector<web::uri> m_parsed_urls;

};

void CCppRestSdk ::DoBenchmark() {
    for (const auto &raw_url: m_raw_urls) {
        try {
            m_parsed_urls.emplace_back(web::uri(raw_url));
        }
        catch (web::uri_exception&) {
            m_invalid_urls.push_back(raw_url);
        }
    }

}

std::string CCppRestSdk ::Name() {
    return "C++ Rest SDK (Casablanca)";
}

#endif //BENCHMARK_URL_CPPRESTSDK_HPP
