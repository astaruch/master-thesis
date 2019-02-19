#ifndef BENCHMARK_URL_SKYRURL_HPP
#define BENCHMARK_URL_SKYRURL_HPP

#include <skyr/url.hpp>

#include "IBenchmark.h"

class CSkyrUrl : public IBenchmark {
public:
    explicit CSkyrUrl(std::vector<std::string> urls)
            : IBenchmark(std::move(urls)) {}

    using url_type = skyr::expected<skyr::url, std::error_code>;

    void DoBenchmark() override;

    std::string Name() override;

    size_t ParsedSize() override { return m_parsed_urls.size(); }

    const auto &Parsed() { return m_parsed_urls; }

    size_t BytesSize() override { return sizeof(std::vector<url_type>) + (sizeof(url_type) * m_parsed_urls.size()); }
private:
    std::vector<url_type> m_parsed_urls;

};

void CSkyrUrl::DoBenchmark() {
    for (const auto &url: m_raw_urls) {
        url_type parsed_url = skyr::make_url(url);
        if (parsed_url.has_value()) {
            m_parsed_urls.push_back(parsed_url);
        } else {
            m_invalid_urls.push_back(url);
        }
    }
}

std::string CSkyrUrl::Name() {
    return "cpp-netlib/url (Skyr URL)";
}

#endif //BENCHMARK_URL_SKYRURL_HPP
