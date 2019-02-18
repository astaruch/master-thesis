#ifndef BENCHMARK_URL_QT_HPP
#define BENCHMARK_URL_QT_HPP

#include <qt5/QtCore/QUrl.h>

#include "IBenchmark.h"

class CQt : public IBenchmark {
public:
    explicit CQt(std::vector<std::string> urls)
            : IBenchmark(std::move(urls)) {}

    void DoBenchmark() override;

    std::string Name() override;

    size_t ParsedSize() override { return m_parsed_urls.size(); }

    const auto &Parsed() { return m_parsed_urls; }

    size_t BytesSize() override { return sizeof(std::vector<QUrl>) + (sizeof(QUrl) * m_parsed_urls.size()); }

private:
    std::vector<QUrl> m_parsed_urls;
};

void CQt::DoBenchmark() {
    for (const std::string &raw_url: m_raw_urls) {
        try {
            m_parsed_urls.emplace_back(QUrl(raw_url));
        } catch (...) {
            m_invalid_urls.push_back(raw_url);
        }
    }
}

std::string CQt::Name() {
    return "Qt";
}

#endif //BENCHMARK_URL_QT_HPP
