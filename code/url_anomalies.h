#ifndef PHISHING_APP_URL_ANOMALIES_H
#define PHISHING_APP_URL_ANOMALIES_H


#include <string>
#include <array>
#include "urllib-cpp/URLlib.h"
#include "third-party/lurlparser/LUrlParser.h"

class url_anomalies {
public:



    // TODO: get this class ready for meta-programming
    void set_url(const std::string& url) { m_url = url; }
    void parse_url(const std::string& url);

    // 4.2.1 Přílišná délka URL
    int check_length(size_t allowed_length) const;

    // 4.2.2 Přílišná hloubka URL
    int check_depth(size_t allowed_dots) const;

    static constexpr std::array<const char*, 8> keywords {
        "webscr", "secure", "banking", "ebayisapi",
        "account", "confirm", "login", "signin"
    };
    // 4.2.4 Klíčová slova a pojmenované entity
    int check_keywords() const ;

    int perform_tests(size_t allowed_length, size_t allowed_depth) const;


private:
    std::string m_url;

    URLlib::URL m_parsed_url;
};


#endif //PHISHING_APP_URL_ANOMALIES_H
