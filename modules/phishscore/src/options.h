#pragma once

#ifndef PHISHSVC_OPTIONS_H
#define PHISHSVC_OPTIONS_H

#include <string>

namespace phishscore {

struct options {
    struct {
        std::string host{};
        std::string port{};
        std::string user{};
        std::string password{};
        std::string conn_string{};
        std::string db_name{};
    } database;

    struct {
        bool stdin{false};
        std::string url{};
    } input;

    struct training_data_input {
        bool stdin{false};
        std::string url{};
    } td;

    struct {
        uint64_t all{0};
        uint64_t url{0};
        uint64_t html{0};
        uint64_t host_based{0};
    } flags;

    struct {
        bool include_url{false};
        bool extra_values{false};
        double class_label{-1};
    } fvec;

    struct {
        std::string bin_path{};
        std::string host{"localhost"};
        uint16_t port{0};
    } html_analysis;

    struct {
        std::string path{};
        std::string host{"localhost"};
        uint16_t port{0};
    } model_checker;

    std::string parse_urls_to_table{};
    bool verbose{false};
};

} // phishscore

#endif // PHISHSVC_OPTIONS_H
