#pragma once

#ifndef PHISHSVC_OPTIONS_H
#define PHISHSVC_OPTIONS_H

#include <string>

struct options_t {
    struct database_t {
        std::string host{};
        std::string port{};
        std::string user{};
        std::string password{};
        std::string conn_string{};
        std::string db_name{};
    } database;

    std::string parse_urls_to_table{};
};

#endif // PHISHSVC_OPTIONS_H
