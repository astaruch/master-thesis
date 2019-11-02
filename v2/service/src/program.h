#pragma once

#ifndef PHISHSVC_PROGRAM_H
#define PHISHSVC_PROGRAM_H

#include <string_view>

#include <cxxopts.hpp>

#include "features/feature_base.h"

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
    uint64_t feature_flags();
    uint64_t html_feature_flags();

    /// HTML features options
    std::string_view node_bin();
    std::string_view html_script();

    /// Training data options
    std::string training_data_url();
    std::string training_data_output_name();
    bool create_training_data();
    double training_data_class_value();

    bool parse_urls();
    std::string table_name();
private:
    cxxopts::Options _options;

    /// General options
    bool _help;
    bool _version;

    /// Disambiguation options
    bool _enable_features;
    bool _enable_database;
    bool _enable_table_manipulation;
    bool _enable_training_data;

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

    uint64_t _feature_flags{0};
    uint64_t _html_feature_flags{0};

    /// HTML features settings
    std::string _node_bin;
    std::string _html_script;

    /// Training data
    std::string _training_data_url;
    bool _training_data_stdin;
    std::string _training_data_input_file;
    std::string _training_data_output_name;
    double _training_data_class_value;
    bool _missing_training_data_class_value;


    const char* on_off(bool feature);
    void check_feature_option(bool feature_on, uint64_t feature_id, std::string_view feature_name);
    void check_html_feature_option(bool feature_on, uint64_t feature_id, std::string_view feature_name);
};

#endif // PHISHSVC_PROGRAM_H
