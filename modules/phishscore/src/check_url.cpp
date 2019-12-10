#include "check_url.h"
#include "env.h"
#include <spdlog/spdlog.h>
#include "safebrowsing_api.h"
#include "training_data.h"
#include "model_checker.h"

namespace phishscore {

using nlohmann::json;

nlohmann::json check_url(const options& opts, const std::string& url, database& db)
{
    auto score = db.check_phishing_score(url);
    json obj = json::object();
    obj["url"] = url;
    // 1. check whether we have phishing score in our cache
    if (score != -1) {
        obj["score"] = score;
        return obj;
    }
    // 2a. check our local phishtank instance
    bool unsafe = false;
    if (db.table_exists("phishtank")) {
        unsafe = db.check_url_in_phishtank(url);
        if (unsafe) {
            score = 100;
            obj["score"] = score;
            return obj;
        }
    }

    // 3. query google safebrowsing
    auto sb_api_key = get_env_var("GOOGLE_SAFEBROWSING_API_KEY");
    if (!sb_api_key.empty()) {
        spdlog::info("Checking URL in Google Safebrowsing");
        safebrowsing_api sb_api(sb_api_key);
        unsafe = sb_api.check_unsafe_url(url);
        if (unsafe) {
            score = 100;
            obj["score"] = score;
            return obj;
        }
        spdlog::info("URL {}found", unsafe ? "" : "not ");
    } else {
        spdlog::warn("GOOGLE_SAFEBROWSING_API_KEY not set. Skipping querying Safebrowsing API");
    }

    // 4. check our model prediction
    spdlog::info("Checking URL in the pretrained heuristic model");
    phishscore::training_data td(opts);
    td.set_input_data({url});
    const auto data = td.get_data_for_model();
    phishscore::model_checker_t model(opts);

    if (data.empty()) {
        spdlog::error("Unknown error");
        nlohmann::json j = nlohmann::json::object();
        j["error"] = "MODEL_CHECKER";
        j["message"] = "No data";
        return j;
    }

    json data_json(data.front());
    if (opts.verbose) fmt::print("{}\n", data_json.dump());
    auto response = model.predict(data_json);
    if (response.find("error") != response.end()) {
        spdlog::error("Error occured: {}", response.dump());
        return response;
    }
    score = response["score"].get<int>();
    obj["score"] = score;

    // 5. store our results to a cache (db)
    db.store_phishing_score(url, score);
    return obj;
}

} // namespace phishscore
