#ifndef BENCHMARK_URL_URIPARSER_H
#define BENCHMARK_URL_URIPARSER_H

#include <uriparser/Uri.h>

#include "IBenchmark.h"
class CUriparser: public IBenchmark {
public:
    explicit CUriparser(std::vector<std::string> urls)
            :IBenchmark(std::move(urls)) { }

    void DoBenchmark() override;

    std::string Name() override;
    size_t ParsedSize() override { return m_parsed_urls.size(); }

private:
    std::vector<UriUriA> m_parsed_urls;
};

void CUriparser::DoBenchmark()
{
    for (const auto& url: m_raw_urls) {
        UriUriA parsed_url;
        const char* const cstr_url = url.c_str();
        const char* error;
        if (uriParseSingleUriA(&parsed_url, cstr_url, &error) != URI_SUCCESS) {
            m_invalid_urls.push_back(url);
            continue;
        }
        m_parsed_urls.push_back(parsed_url);
        uriFreeUriMembersA(&parsed_url);
    }
}

std::string CUriparser::Name()
{
    return "uriparser";
}

#endif //BENCHMARK_URL_URIPARSER_H
