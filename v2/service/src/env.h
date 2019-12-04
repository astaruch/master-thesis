#pragma once

#ifndef PHISHCV_ENV_H
#define PHISHCV_ENV_H

#include <string>
#include <cstdlib>

// std::string get_env_var(const std::string& key)
// {
//     char* val = std::getenv(key.c_str());
//     return val ? std::string(val) : std::string();
// }

std::string get_env_var(const std::string& key, const std::string& fallback = "")
{
    char* val = std::getenv(key.c_str());
    return val ? std::string(val) : fallback;
}

#endif // PHISHCV_ENV_H
