#ifndef BENCHMARK_URL_SKYRURL_HPP
#define BENCHMARK_URL_SKYRURL_HPP

#include <skyr/url.hpp>

#include "IBenchmark.h"

class SkyrUrl: public IBenchmark {
public:
    explicit SkyrUrl(std::vector<std::string> urls)
            :IBenchmark(std::move(urls)) { }

    using url_type = skyr::expected<skyr::url, std::error_code>;

    void DoBenchmark() override;
    std::string Name() override;
private:
    std::vector<url_type> m_parsed_urls;

};

void SkyrUrl::DoBenchmark()
{
    std::transform(m_raw_urls.begin(), m_raw_urls.end(), std::back_inserter(m_parsed_urls),
            [](const std::string url) -> url_type {
                return skyr::make_url(url);
            });

    std::vector<url_type> invalid_urls;

    url_type url = skyr::make_url("\xf0\x9f\x8d\xa3\xf0\x9f\x8d\xba");
    if (!url) {
        std::cout << "Invalid url\n";
    }
//    m_parsed_urls.push_back(url);
    std::copy_if(m_parsed_urls.begin(), m_parsed_urls.end(), std::back_inserter(invalid_urls),
            [](const url_type& url) -> bool {
                return !url;
            });

    std::cout << "Invalid urls: " << invalid_urls.size() << "\n";
}

std::string SkyrUrl::Name()
{
    return "SkyrUrl";
}

#endif //BENCHMARK_URL_SKYRURL_HPP
