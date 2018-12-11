#ifndef PHISHING_APP_URL_ANOMALIES_H
#define PHISHING_APP_URL_ANOMALIES_H


#include <string>
#include <array>
#include "third-party/lurlparser/LUrlParser.h"

class url_anomalies {
public:
    class parser {
    public:
        // TODO: use this go parser - https://github.com/golang/go/blob/master/src/net/url/url.go
        bool parse(const std::string& uri);
        const std::string& uri() const { return m_uri; }
        const std::string& scheme() const { return m_scheme; }
        const std::string& authority() const { return m_authority; }
        const std::string& path() const { return m_path; }
        const std::string& params() const { return m_params; }
        const std::string& query() const { return m_query; }
        const std::string& fragments() const { return m_fragment; }
        const std::string& username() const { return m_username; }
        const std::string& password() const { return m_password; }
        const std::string& hostname() const { return m_hostname; }
        const size_t& port() const { return m_port; }

        // Maybe rawurl is of the form scheme:path.
        // (Scheme must be [a-zA-Z][a-zA-Z0-9+-.]*)
        // If so, return scheme, path; else return "", rawurl.
        bool parse_scheme(const std::string& uri);


        bool parse_authority(const std::string& authority);

        // parseHost parses host as an authority without user
        // information. That is, as host[:port].
        bool parse_host(const std::string& host);
    private:
        std::string m_uri;
        // [scheme:][//[userinfo@]host][/]path[?query][#fragment]

        std::string m_scheme;
        std::string m_authority; //host+port
        std::string m_path;
        std::string m_params;
        std::string m_query;
        std::string m_fragment;
        std::string m_username;
        std::string m_password;
        std::string m_hostname;
        size_t m_port;
    };
    // TODO: dont use LUrlParser and finish my own


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

    LUrlParser::clParseURL m_parsed_url;


};


#endif //PHISHING_APP_URL_ANOMALIES_H
