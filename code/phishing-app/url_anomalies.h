#ifndef PHISHING_APP_URL_ANOMALIES_H
#define PHISHING_APP_URL_ANOMALIES_H


#include <string>

class url_anomalies {
public:
    class parser {
    public:
        // TODO: use this go parser - https://github.com/golang/go/blob/master/src/net/url/url.go
        bool parse(const std::string& uri);
        const std::string& uri() const { return m_uri; }
        const std::string& scheme() const { return m_scheme; }
        const std::string& netloc() const { return m_netloc; }
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
    private:
        std::string m_uri;
        // [scheme:][//[userinfo@]host][/]path[?query][#fragment]

        std::string m_scheme;
        std::string m_netloc;
        std::string m_path;
        std::string m_params;
        std::string m_query;
        std::string m_fragment;
        std::string m_username;
        std::string m_password;
        std::string m_hostname;
        size_t m_port;
    };

    // TODO: get this class ready for meta-programming
    void set_url(const std::string& url) { m_url = url; }

    // 4.2.1 Přílišná délka URL
    int check_length(size_t length) const;

    // 4.2.2 Přílišná hloubka URL
    int check_depth(size_t depth) const;

    // 4.2.3 Speciální znaky
    int check_special_chars();


private:
    std::string m_url;


};


#endif //PHISHING_APP_URL_ANOMALIES_H
