#ifndef PHISHSVC_SAFEBROWSING_API_H
#define PHISHSVC_SAFEBROWSING_API_H

#include <string>
#include <curl/curl.h>

#include <nlohmann/json.hpp>

using json = nlohmann::json;


class safebrowsing_api {
public:
    safebrowsing_api();

    safebrowsing_api(const std::string& key);

    ~safebrowsing_api();

    enum class threat_type {
        unspecified,
        malware,
        social_engineering,
        unwanted_software,
        potentailly_harmful_application
    };

    enum class platform_type {
        unspecified,
        windows,
        linux_t,
        android,
        osx,
        ios,
        any,
        all,
        chrome
    };

    enum class threat_entry_type {
        unspecified,
        url,
        executable
    };

    enum class rest_resource {
        full_hashes,
        threat_list_updates,
        threat_lists,
        threat_matches
    };

    void set_key(const std::string& key) {
        m_key = key;
    }
    bool check_unsafe_url(const std::string& url);

    void add_threat_type(const threat_type& type);

    void add_platform_type(const platform_type& type);

    void add_threat_entry_type(const threat_entry_type& type);

    void add_url(const std::string& url);

    json threat_matches();

    void init_cfg();

    bool evaluate_response(json response);

    void dump();

    void clear_urls();

    size_t number_of_urls() const;

private:
    std::string m_key;
    CURL* m_curl;
    json m_cfg;

    const std::string m_api_base = "https://safebrowsing.googleapis.com/v4";



};

#endif //PHISHSVC_SAFEBROWSING_API_H
