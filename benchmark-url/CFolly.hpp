#ifndef BENCHMARK_URL_FOLLY_HPP
#define BENCHMARK_URL_FOLLY_HPP

#include <folly/Uri.h>

#include "IBenchmark.h"

class CFolly : public IBenchmark {
public:
    explicit CFolly(std::vector<std::string> urls)
            : IBenchmark(std::move(urls)) {}

    void DoBenchmark() override;

    std::string Name() override;

    size_t ParsedSize() override { return m_parsed_urls.size(); }

    const auto &Parsed() { return m_parsed_urls; }

    size_t BytesSize() override { return sizeof(std::vector<folly::Uri>) + (sizeof(folly::Uri) * m_parsed_urls.size()); }

private:
    std::vector<folly::Uri> m_parsed_urls;
};

void CFolly::DoBenchmark() {
    for (const std::string &raw_url: m_raw_urls) {
        try {
            m_parsed_urls.emplace_back(folly::Uri(raw_url));
        } catch (std::invalid_argument &) {
            m_invalid_urls.push_back(raw_url);
        }
    }
}

std::string CFolly::Name() {
    return "Folly (Facebook)";
}

#endif //BENCHMARK_URL_FOLLY_HPP
