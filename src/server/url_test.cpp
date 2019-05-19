#include "spdlog/spdlog.h"
#include "Poco/Exception.h"
#include "Poco/StringTokenizer.h"

#include "libs/config.h"
#include "url_test.h"

namespace phishapp {

Url_test::Url_test(const std::string& raw_url)
        :m_raw_url(raw_url),
         m_initialized(false)
{
    spdlog::info("setting url for tests: '{}'", raw_url);

    try {
        spdlog::info("parsing url");
        m_url = Poco::URI(raw_url);
    }
    catch (const Poco::Exception& ex) {
        spdlog::error("can't parse URL: {} - {}", ex.what(), ex.message());
        return;
    }
    spdlog::info("parsed");
    spdlog::info("creating additional members");

    if (m_url.getHost().empty()) {
        spdlog::error("url is malformed");
        return;
    }
    auto labels = Poco::StringTokenizer(m_url.getHost(), ".");
    m_components = std::vector<std::string>(labels.begin(), labels.end());
    m_tld = m_components.back();
    m_components.pop_back();

    // reverse components so we have most relevant domain on zero position
    std::reverse(m_components.begin(), m_components.end());
    m_sld = m_components.front();
    spdlog::info("done");
    m_initialized = true;
}

int
Url_test::test_length() const
{
    int weight = Config::weight_length();
    int max_length = Config::params_max_length();
    return m_raw_url.size() > max_length ? weight : -weight;
}

int
Url_test::test_depth() const
{
    int weight = Config::weight_depth();
    int max_depth = Config::params_max_depth();
    return m_components.size() > max_depth ? weight : -weight;
}

int
Url_test::do_tests()
{
    if (!m_initialized)
    {
        spdlog::warn("can't run tests - given URL is invalid");
        return 0;
    }
    int score = 0;
    if (Config::test_length())
    {
        score += test_length();
    }

    if (Config::test_depth())
    {
        score += test_depth();
    }

    // TODO: all other tests
    return score;
}

} // namespace phishapp
