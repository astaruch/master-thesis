#pragma once

#ifndef PHISHSVC_PROGRAM_H
#define PHISHSVC_PROGRAM_H

#include <string_view>

#include <cxxopts.hpp>

class program {
public:

    program(int argc, char** argv);

    /// Performs general operations from user input
    /**
     * This method will perform general checking, whether user provided
     * all information needed, or how the application should behave, and
     * prepare for the next steps.
     */
    void check_options();

    /// Gets or construct a connection string to a database
    std::string get_conn_string();

    /// Check whether we are going to manipulating with a table
    bool table_manipulation();

    /// Check whether we are going to work with a features
    bool features_enabled();

    /// Return flags with features
    uint64_t feature_flags() const;
    uint64_t url_feature_flags() const;
    uint64_t html_feature_flags() const;
    uint64_t host_based_feature_flags() const;

    /// Training data options
    bool create_training_data();

    bool parse_urls();
    std::string table_name();

    /// Training data
    std::string _training_data_url{};
    bool training_data_stdin{false};
    std::string _training_data_input_file{};
    std::string _training_data_output_name{};
    bool _training_data_output_stdout{false};
    double _training_data_class_value{0};
    bool _missing_training_data_class_value;
    bool output_include_url{false};

    // General options
    bool help_{false};
    bool version_{false};
    bool verbose{false};

    /// HTML features settings
    std::string node_bin{};
    std::string html_script{};
    std::string htmlfeatures_bin{};
private:
    cxxopts::Options _options;

    /// Disambiguation options
    bool _enable_features;
    bool _enable_database;
    bool _enable_table_manipulation;
    bool _enable_training_data;
    bool _enable_intermediate_value;

    std::string _host;
    std::string _port;
    std::string _dbname;
    std::string _user;
    std::string _password;
    std::string _conn_string;

    /// Table manipulation options
    bool _table_manipulation;
    std::string _table;
    bool _parse_urls;

    /// Features
    // url
    bool _feature_ip_address{false};
    bool _feature_url_length{false};
    bool _feature_host_length{false};
    bool _feature_path_length{false};
    bool _feature_query_length{false};
    bool _feature_fragment_length{false};
    bool _feature_user_info{false};
    bool _feature_domain_count{false};
    bool _feature_https_used{false};
    bool _feature_extra_https{false};
    bool _feature_shortening_service{false};
    bool _feature_non_std_port{false};
    bool _feature_spec_chars_path{false};
    bool _feature_spec_chars_query{false};
    bool _feature_spec_chars_fragment{false};
    bool _feature_spec_chars_host{false};
    bool _feature_gtld{false};
    bool _feature_www_prefix{false};
    bool _feature_four_numbers{false};
    bool _feature_spec_keywords{false};
    bool _feature_punycode{false};
    // html
    bool _feature_input_tag{false};
    bool _feature_src_link{false};
    bool _feature_form_handler{false};
    bool _feature_invisible_iframe{false};
    bool _feature_rewrite_statusbar{false};
    bool _feature_disable_rightclick{false};
    bool _feature_ahref_link{false};
    bool _feature_popup_window{false};
    bool _feature_favicon_link{false};
    bool _feature_old_technologies{false};
    bool _feature_missleading_link{false};
    bool _feature_hostname_title{false};
    // host based
    bool _feature_redirect{false};
    bool _feature_google_index{false};
    bool _feature_dns_a_record{false};
    bool _feature_dnssec{false};
    bool _feature_dns_created{false};
    bool _feature_dns_updated{false};
    bool _feature_ssl_created{false};
    bool _feature_ssl_expire{false};
    bool _feature_ssl_subject{false};
    bool _feature_hsts{false};
    bool _feature_xss_protection{false};
    bool _feature_csp{false};
    bool _feature_x_frame{false};
    bool _feature_x_content_type{false};
    bool _feature_asn{false};
    bool _feature_similar_domain{false};

    uint64_t _feature_flags{0};
    uint64_t _url_feature_flags{0};
    uint64_t _html_feature_flags{0};
    uint64_t _host_based_feature_flags{0};



    const char* on_off(bool feature);
    void check_feature_option(bool feature_on, uint64_t feature_id, std::string_view feature_name);
    void check_url_feature_option(bool feature_on, uint64_t feature_id, std::string_view feature_name);
    void check_html_feature_option(bool feature_on, uint64_t feature_id, std::string_view feature_name);
    void check_host_based_feature_option(bool feature_on, uint64_t feature_id, std::string_view feature_name);
};

#endif // PHISHSVC_PROGRAM_H
