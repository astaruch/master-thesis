#include "safebrowsing_api.h"

#include <spdlog/spdlog.h>

#include <iostream>

safebrowsing_api::safebrowsing_api()
    : safebrowsing_api(std::string())
{
}

safebrowsing_api::safebrowsing_api(const std::string& key)
    : m_key(key)
{
    curl_global_init(CURL_GLOBAL_ALL);
    m_curl = curl_easy_init();
    init_cfg();
}

safebrowsing_api::~safebrowsing_api()
{
    curl_easy_cleanup(m_curl);
}

void safebrowsing_api::add_threat_type(const safebrowsing_api::threat_type& type)
{
    auto& types = m_cfg["threatInfo"]["threatTypes"];
    switch (type) {
        case threat_type::unspecified:
            types.push_back("THREAT_TYPE_UNSPECIFIED");
            break;
        case threat_type::malware:
            types.push_back("MALWARE");
            break;
        case threat_type::social_engineering:
            types.push_back("SOCIAL_ENGINEERING");
            break;
        case threat_type::unwanted_software:
            types.push_back("UNWANTED_SOFTWARE");
            break;
        case threat_type::potentailly_harmful_application:
            types.push_back("POTENTIALLY_HARMFUL_APPLICATION");
            break;
    }
}

void safebrowsing_api::add_platform_type(const safebrowsing_api::platform_type& type)
{
    auto& types = m_cfg["threatInfo"]["platformTypes"];
    switch (type) {
        case platform_type::unspecified:
            types.push_back("PLATFORM_TYPE_UNSPECIFIED");
            break;
        case platform_type::windows:
            types.push_back("WINDOWS");
            break;
        case platform_type::linux_t:
            types.push_back("LINUX");
            break;
        case platform_type::android:
            types.push_back("ANDROID");
            break;
        case platform_type::osx:
            types.push_back("OSX");
            break;
        case platform_type::ios:
            types.push_back("IOS");
            break;
        case platform_type::any:
            types.push_back("ANY_PLATFORM");
            break;
        case platform_type::all:
            types.push_back("ALL_PLATFORMS");
            break;
        case platform_type::chrome:
            types.push_back("CHROME");
            break;
    }
}

void safebrowsing_api::add_threat_entry_type(const safebrowsing_api::threat_entry_type& type)
{
    auto& types = m_cfg["threatInfo"]["threatEntryTypes"];
    switch (type) {
        case threat_entry_type::unspecified:
            types.push_back("THREAT_ENTRY_TYPE_UNSPECIFIED");
            break;
        case threat_entry_type::url:
            types.push_back("URL");
            break;
        case threat_entry_type::executable:
            types.push_back("EXECUTABLE");
            break;
    }
}

void safebrowsing_api::add_url(const std::string& url)
{
    auto& entries = m_cfg["threatInfo"]["threatEntries"];
    entries.push_back({ {"url", url} });
}

void safebrowsing_api::init_cfg()
{
    m_cfg = {
        {"client",
            {
                {"clientId",    "libsafebrowsing_api-cpp"},
                {"clientVersion", "0.1"}
            }
        },
        {"threatInfo",
            {
                {"threatTypes", json::array()},
                {"platformTypes", json::array()},
                {"threatEntryTypes", json::array()},
                {"threatEntries", json::array()}
            }
        }
    };
    add_threat_type(safebrowsing_api::threat_type::unwanted_software);
    add_threat_type(safebrowsing_api::threat_type::malware);
    add_threat_type(safebrowsing_api::threat_type::social_engineering);
    add_platform_type(safebrowsing_api::platform_type::all);
    add_threat_entry_type(safebrowsing_api::threat_entry_type::url);
}

static size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp)
{
    (static_cast<std::string*>(userp))->append(static_cast<char*>(contents), size * nmemb);
    return size * nmemb;
}

json safebrowsing_api::threat_matches()
{
    auto api_url = m_api_base + "/threatMatches:find?key=" + m_key;

    if (!m_curl) {
        exit(1);
    }
    curl_easy_setopt(m_curl, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(m_curl, CURLOPT_URL, api_url.c_str());
    curl_easy_setopt(m_curl, CURLOPT_POST, 1);

    std::string body = m_cfg.dump();
    curl_easy_setopt(m_curl, CURLOPT_POSTFIELDS, body.c_str());
    curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYPEER, 0L);

    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    curl_easy_setopt(m_curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, write_callback);

    std::string buffer;
    curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, &buffer);

    CURLcode res = curl_easy_perform(m_curl);
    if (res != CURLE_OK) {
        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << '\n';
    }
    return json::parse(buffer);
}

bool safebrowsing_api::evaluate_response(json response)
{
    std::cout << response.dump(2) << '\n';
    if (response.empty()) {
        std::cout << "Safe.\n";
        return true;
    }
    std::cerr << "Unsafe.\n";
    return false;
}

void safebrowsing_api::dump()
{
    std::cout << m_cfg.dump(2) << '\n';
}

void safebrowsing_api::clear_urls()
{
    m_cfg["threatInfo"]["threatEntries"].clear();
}

size_t safebrowsing_api::number_of_urls() const
{
    return m_cfg["threatInfo"]["threatEntries"].size();
}

bool safebrowsing_api::check_unsafe_url(const std::string& url)
{
    clear_urls();
    add_url(url);
    auto res = threat_matches();
    return res.empty() ? false : true;
}
