#pragma once

#ifndef PHISHSCORE_CHECK_URL_H
#define PHISHSCORE_CHECK_URL_H

#include "options.h"
#include "database.h"

#include <nlohmann/json.hpp>
#include <string>

namespace phishscore {

nlohmann::json check_url(const options& opts, const std::string& url, database& db);

} // namespace phishscore

#endif // PHISHSCORE_CHECK_URL_H
