#pragma once

#ifndef PHISHSVC_OPTIONS_H
#define PHISHSVC_OPTIONS_H

#include <string>

namespace phishscore {

struct options_t {
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

    std::string parse_urls_to_table{};
};

} // phishscore

#endif // PHISHSVC_OPTIONS_H
