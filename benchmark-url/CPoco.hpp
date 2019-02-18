#ifndef BENCHMARK_URL_POCO_HPP
#define BENCHMARK_URL_POCO_HPP

#include <Poco/URI.h>
#include <Poco/Exception.h>

#include "IBenchmark.h"

class CPoco : public IBenchmark {
public:
    explicit CPoco(std::vector<std::string> urls)
            : IBenchmark(std::move(urls)) {}

    void DoBenchmark() override;

    std::string Name() override;

    size_t ParsedSize() override { return m_parsed_urls.size(); }

    const auto &Parsed() { return m_parsed_urls; }

    size_t BytesSize() override { return sizeof(std::vector<Poco::URI>) + (sizeof(Poco::URI) * m_parsed_urls.size()); }

private:
    std::vector<Poco::URI> m_parsed_urls;
};

void CPoco::DoBenchmark() {
    for (const std::string &raw_url: m_raw_urls) {
        try {
            m_parsed_urls.emplace_back(Poco::URI(raw_url));
        } catch (Poco::SyntaxException &) {
            m_invalid_urls.push_back(raw_url);
        }
    }
}

std::string CPoco::Name() {
    return "Poco";
}

#endif //BENCHMARK_URL_POCO_HPP
