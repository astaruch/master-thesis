#include "training_data.h"
#include "features/feature_enum.h"
#include "feature_vector.h"
#include "features/html_features.h"

#include <algorithm>
#include <numeric>

#include <fmt/format.h>

namespace phishscore {

training_data::training_data(const options& opts)
    : opts_(opts)
{
}

void training_data::set_input_data(std::vector<std::string> urls)
{
    _urls = urls;
}

bool training_data::print_csv_training_data()
{

    std::string csv_header = opts_.fvec.include_url ? "url," : "";
    csv_header += create_csv_header();
    fmt::print("{}\n", csv_header);

    for (const auto& url: _urls) {
        feature_vector fvec(opts_, url);
        fvec.print_in_csv_format();
    }

    return true;
}

std::string training_data::create_csv_header()
{
    std::vector<std::string> columns;

    for (const auto id: feature_enum::url) {
        if (opts_.flags.url & id) {
            columns.push_back(std::string(feature_enum::column_names.at(id)));
        }
    }

    if (opts_.flags.html) {
        html_features_t html("", opts_.flags.html, opts_.html_analysis.bin_path, opts_.fvec.extra_values);
        auto html_header = html.get_header();
        size_t pos = 0;
        std::string token;
        while ((pos = html_header.find(',')) != std::string::npos) {
            token = html_header.substr(0, pos);
            columns.push_back(token);
            html_header.erase(0, pos + 1);
        }
        if (!html_header.empty()) {
            columns.push_back(html_header);
        }
    }

    for (const auto id: feature_enum::host_based) {
        if (opts_.flags.host_based & id) {
            columns.push_back(std::string(feature_enum::column_names.at(id)));
        }
    }

    if (opts_.fvec.extra_values) {
        if (opts_.flags.host_based & feature_enum::dns_created) {
            columns.push_back(std::string(feature_enum::column_names.at(feature_enum::dns_created)) + "_value");
        }
        if (opts_.flags.host_based & feature_enum::dns_updated) {
            columns.push_back(std::string(feature_enum::column_names.at(feature_enum::dns_updated)) + "_value");
        }
        if (opts_.flags.host_based & feature_enum::ssl_created) {
            columns.push_back(std::string(feature_enum::column_names.at(feature_enum::ssl_created)) + "_value");
        }
        if (opts_.flags.host_based & feature_enum::ssl_expire) {
            columns.push_back(std::string(feature_enum::column_names.at(feature_enum::ssl_expire)) + "_value");
        }
        if (opts_.flags.host_based & feature_enum::asn) {
            columns.push_back(std::string(feature_enum::column_names.at(feature_enum::asn)) + "_value");
        }
    }
    columns.push_back("label");

    return std::accumulate(columns.begin(), columns.end(), std::string(),
        [](const std::string& a, const std::string& b) -> std::string {
            return a + (a.length() > 0 ? "," : "") + b;
        }
    );
}

std::vector<std::unordered_map<std::string_view, double>> training_data::get_data_for_model()
{
    std::vector<std::unordered_map<std::string_view, double>> data;
    for (const auto& url: _urls) {
        feature_vector fvec(opts_, url);
        data.push_back(fvec.compute_values());
    }
    return data;
}

} // namespace phishscore
