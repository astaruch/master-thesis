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
    bool _feature_ip_address;
    bool _feature_url_length;
    bool _feature_host_length;
    bool _feature_path_length;
    bool _feature_query_length;
    bool _feature_fragment_length;
    bool _feature_user_info;
    bool _feature_domain_count;
    bool _feature_https_used;
    bool _feature_extra_https;
    bool _feature_shortening_service;
    bool _feature_non_std_port;
    bool _feature_spec_chars_path;
    bool _feature_spec_chars_query;
    bool _feature_spec_chars_fragment;
    bool _feature_spec_chars_host;
    bool _feature_gtld;
    bool _feature_www_prefix;
    bool _feature_four_numbers;

    uint64_t _feature_flags;

    /// Training data
    std::string _training_data_url;
    bool _training_data_stdin;
    std::string _training_data_input_file;
    std::string _training_data_output_name;
    double _training_data_class_value;
    bool _missing_training_data_class_value;


    const char* on_off(bool feature);
    void check_feature_option(bool feature_on, uint64_t feature_id, std::string_view feature_name);
};

#endif // PHISHSVC_PROGRAM_H
