#pragma once

#ifndef PHISHSVC_MODEL_CHECKER_H
#define PHISHSVC_MODEL_CHECKER_H

#include "options.h"

#include <string_view>

#include <nlohmann/json.hpp>

namespace phishscore {

class model_checker_t {
public:
    model_checker_t(const options& opts);

    nlohmann::json predict(const nlohmann::json& json);
    nlohmann::json use_program(const std::string& escaped_json);
    nlohmann::json use_service(const std::string& json_str);
private:
    const options opts_;
};

}

#endif // PHISHSVC_MODEL_CHECKER_H
