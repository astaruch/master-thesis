#pragma once

#ifndef PHISHSVC_MODEL_CHECKER_H
#define PHISHSVC_MODEL_CHECKER_H

#include <string_view>

#include <nlohmann/json.hpp>

class model_checker_t {
public:
    model_checker_t(std::string_view path, uint16_t port);

    nlohmann::json predict(const nlohmann::json& json);
    nlohmann::json use_program(const std::string& escaped_json);
    nlohmann::json use_service(const std::string& json_str);
private:
    std::string_view path_;
    uint16_t port_;
};

#endif // PHISHSVC_MODEL_CHECKER_H
