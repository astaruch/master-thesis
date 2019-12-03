#pragma once

#ifndef PHISHSVC_MODEL_CHECKER_H
#define PHISHSVC_MODEL_CHECKER_H

#include <string_view>

class model_checker_t {
public:
    model_checker_t(std::string_view path);

    double predict(const std::string& unescaped_json);
private:
    std::string_view path_;
};

#endif // PHISHSVC_MODEL_CHECKER_H
