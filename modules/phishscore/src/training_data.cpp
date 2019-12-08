#include "training_data.h"

#include "features/feature_enum.h"
#include "features/url_features.h"
#include "features/html_features.h"
#include "features/host_based_features.h"

#include "feature_vector.h"

#include <algorithm>
#include <numeric>

#include <spdlog/spdlog.h>
#include <Poco/URI.h>
#include <Poco/Exception.h>

namespace phishscore {

training_data::training_data(const options& opts)
    : opts_(opts)
{
}

training_data::training_data(bool verbose, bool output_include_url = false, bool output_extra_values = false)
    : verbose_(verbose)
    , output_include_url_(output_include_url)
    , output_extra_values_(output_extra_values)
{
}

void training_data::set_flags(uint64_t all, uint64_t url, uint64_t html, uint64_t host_based)
{
    _feature_flags = all;
    _url_feature_flags = url;
    _html_feature_flags = html;
    _host_based_feature_flags = host_based;
}

void training_data::set_output(std::FILE* file)
{
    file_ = file;
}

void training_data::set_input_data(std::vector<std::string> urls)
{
    _urls = urls;
}

void training_data::set_label(int label)
{
    _label = label;
}
void training_data::set_html_features_opts(std::string_view htmlfeatures_bin, uint16_t port)
{
    htmlfeatures_bin_ = htmlfeatures_bin;
    port_ = port;
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
        // fmt::print("HEADER = {}\n", html_header);
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
        if (verbose_) fmt::print(file_, "--> Checking {}\n", url);
        try {
            const Poco::URI parsed = Poco::URI(url); // throws SyntaxException on bad URL
            std::unordered_map<std::string_view, double> fmap;

            if (_url_feature_flags) {
                url_features_t url_features(url, parsed, _url_feature_flags, true);
                auto values = url_features.compute_values_map();
                fmap.merge(values);
            }

            if (_html_feature_flags) {
                html_features_t html_features(url, _html_feature_flags, htmlfeatures_bin_, port_);
                auto values = html_features.compute_values_map();
                fmap.merge(values);
            }

            if (_host_based_feature_flags) {
                host_based_features_t host_based_features(url, parsed, _host_based_feature_flags);
                auto values = host_based_features.compute_values_map();
                fmap.merge(values);
            }
            data.push_back(fmap);
            if (verbose_) fmt::print(file_, "<-- Inserted record with {} features\n", fmap.size());
        } catch (const Poco::SyntaxException& ex) {
            if (verbose_) fmt::print(stderr, "{}: {}\n", ex.what(), ex.message());
            continue;
        }
    }
    return data;
}

} // namespace phishscore
