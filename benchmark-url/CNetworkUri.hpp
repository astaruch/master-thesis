#ifndef BENCHMARK_URL_NETWORKURI_HPP
#define BENCHMARK_URL_NETWORKURI_HPP

#include <network/uri.hpp>

#include "IBenchmark.h"

class CNetworkUri : public IBenchmark {
public:
    explicit CNetworkUri(std::vector<std::string> urls)
            : IBenchmark(std::move(urls)) {}

    void DoBenchmark() override;

    std::string Name() override;

    size_t ParsedSize() override { return m_parsed_urls.size(); }

    size_t BytesSize() override { return sizeof(std::vector<network::uri>) + (sizeof(network::uri) * m_parsed_urls.size()); }

private:
    std::vector<network::uri> m_parsed_urls;

};

void CNetworkUri::DoBenchmark() {
    for (const auto &raw_url: m_raw_urls) {
        try {
            m_parsed_urls.push_back(network::uri(raw_url));
        }
        catch (network::uri_syntax_error &) {
            m_invalid_urls.push_back(raw_url);
        }
    }
}

std::string CNetworkUri::Name() {
    return "cpp-netlib/uri (C++ Network URI)";
}

#endif //BENCHMARK_URL_NETWORKURI_HPP
