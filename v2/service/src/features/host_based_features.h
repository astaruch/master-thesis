#pragma once

#ifndef PHISHSVC_FEATURE_HOST_BASED_FEATURES_H
#define PHISHSVC_FEATURE_HOST_BASED_FEATURES_H

#include "feature_enum.h"

#include <regex>
#include <string_view>
#include <vector>

#include <Poco/URI.h>

class host_based_features_t {
public:
    host_based_features_t() = default;
    explicit host_based_features_t(const std::string_view url);
    host_based_features_t(const std::string_view url, const Poco::URI& parsed,
        const uint64_t flags, const bool url_is_ok);

    void set_verbose(bool verbose) {
        verbose_ = verbose;
    }

    std::vector<double> compute_values_vec() const;
    std::string extra_values();
    std::vector<std::string> extra_columns();

    std::unordered_map<feature_enum::id, double> compute_values_map() const;
    double compute_value(feature_enum::id feature) const;
    double compute_value_redirect() const;
    double compute_value_google_indexed() const;
    double compute_value_dns_a_record() const;
    double compute_value_dnssec() const;
    double compute_value_dns_created() const;
    double compute_value_dns_updated() const;
    double compute_value_ssl_created() const;
    double compute_value_ssl_expire() const;
    double compute_value_ssl_subject() const;
    double compute_value_hsts() const;
    double compute_value_xss_protection() const;
    double compute_value_csp() const;
    double compute_value_x_frame() const;
    double compute_value_x_content_type() const;
    double compute_value_asn() const;
    double compute_similar_domain() const;
// protected:
    double compute_value_redirect(std::string_view str);
    double compute_value_google_indexed(std::string_view str);
    double compute_value_dns_a_record(std::string_view str); // for testing purpose
    double compute_value_dnssec(std::string_view str); // for testing purpose
    double compute_value_ssl_created(std::string_view str); // for testing purpose
    double compute_value_ssl_expire(std::string_view str); // for testing purpose
    double compute_value_ssl_subject(std::string_view str); // for testing purpose
    double compute_value_hsts(std::string_view str); // for testing purpose
    double compute_value_xss_protection(std::string_view str);  // for testing purpose
    double compute_value_csp(std::string_view str);  // for testing purpose
    double compute_value_x_frame(std::string_view str);  // for testing purpose
    double compute_value_x_content_type(std::string_view str);  // for testing purpose
    double compute_value_asn(std::string_view str); // for testing purpose
    double compute_similar_domain(std::string_view str, std::string_view str2);
    std::string get_asn() const;
    std::string get_dns_created(std::string_view str);
    std::string get_dns_updated(std::string_view str);
private:
    const std::string_view _url;
    const uint64_t _flags{0};
    const Poco::URI _parsed{};
    const bool _url_is_ok{false};

    const std::string_view _user_agent{"user-agent: Mozilla/5.0 (X11; Fedora; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/78.0.3904.70 Safari/537.36"};
    std::string sld_; // second level domain (hostname without TLD and subdomains)

    std::vector<std::string> dig_response_;
    std::vector<std::string> ssl_response_;
    std::vector<std::string> http_resp_headers_;
    std::vector<std::string> whois_response_;

    std::vector<std::string> extra_values_;
    std::vector<std::string> extra_columns_;

    bool google_indexed_;
    std::string similar_domain_;

    bool verbose_;

    std::string extract_value(const std::vector<std::string>& output, const std::regex& reg) const;
    std::string extract_value_from_output(const std::vector<std::string>& output, const std::regex& reg) const;
    bool check_value_in_output(const std::vector<std::string>& output, const std::regex& reg) const;
    std::string extract_dns_date(const std::regex& reg) const;
    std::string get_dns_created() const;
    std::string get_dns_updated() const;
    std::string get_ssl_subject() const;
    std::string get_ssl_created() const;
    std::string get_ssl_expire() const;
    std::vector<std::string>  get_ssl_response() const;
    void fill_ssl_response();
    std::vector<std::string> get_http_resp_headers() const;
    void fill_http_resp_headers();
    std::vector<std::string> get_dig_response() const;
    void fill_dig_response();
    std::vector<std::string> get_whois_response() const;
    void fill_whois_response();
    std::string get_sld() const;
    void fill_sld();
    std::string get_word_suggestion(std::string_view word) const;
    std::string asn_;
    void fill_asn();
    bool get_is_google_indexed() const;

    void fill_google_index();
    std::string get_similar_domain_name() const;
    void fill_similar_domain_name();



    const int timeout_{2};
};

#endif // PHISHSVC_FEATURE_HOST_BASED_FEATURES_H
