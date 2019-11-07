#include "training_data.h"

#include "features/feature_enum.h"
#include "features/features.h"
#include "features/url_features.h"
#include "features/html_features.h"

#include <algorithm>
#include <numeric>

#include <spdlog/spdlog.h>

training_data::training_data(bool verbose)
    : verbose_(verbose)
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

void training_data::set_node_bin(std::string_view node_bin)
{
    _node_bin = node_bin;
}

void training_data::set_html_script(std::string_view html_script)
{
    _html_script = html_script;
}

bool training_data::create_training_data()
{

    std::string csv_header = create_csv_header();
    fmt::print(file_, "{}\n", csv_header);

    transform_urls_to_training_data();

    return true;
}

std::string training_data::create_csv_header()
{
    std::vector<std::string> columns;

    for (const auto id: feature_enum::url) {
        if (_url_feature_flags & id) {
            columns.push_back(std::string(feature_enum::column_names.at(id)));
        }
    }

    for (const auto id: feature_enum::html) {
        if (_html_feature_flags & id) {
            columns.push_back(std::string(feature_enum::column_names.at(id)));
        }
    }

    for (const auto id: feature_enum::host_based) {
        if (_host_based_feature_flags & id) {
            columns.push_back(std::string(feature_enum::column_names.at(id)));
        }
    }

    columns.push_back("label");

    if (verbose_) {
        if (_host_based_feature_flags & feature_enum::dns_created) {
            columns.push_back(std::string(feature_enum::column_names.at(feature_enum::dns_created)) + "_value");
        }
        if (_host_based_feature_flags & feature_enum::dns_updated) {
            columns.push_back(std::string(feature_enum::column_names.at(feature_enum::dns_updated)) + "_value");
        }
        if (_host_based_feature_flags & feature_enum::ssl_created) {
            columns.push_back(std::string(feature_enum::column_names.at(feature_enum::ssl_created)) + "_value");
        }
        if (_host_based_feature_flags & feature_enum::ssl_expire) {
            columns.push_back(std::string(feature_enum::column_names.at(feature_enum::ssl_expire)) + "_value");
        }
        if (_host_based_feature_flags & feature_enum::asn) {
            columns.push_back(std::string(feature_enum::column_names.at(feature_enum::asn)) + "_value");
        }
    }


    return std::accumulate(columns.begin(), columns.end(), std::string(),
        [](const std::string& a, const std::string& b) -> std::string {
            return a + (a.length() > 0 ? "," : "") + b;
        }
    );
}

void training_data::transform_urls_to_training_data()
{
    // takes vector of doubles and transform it to the string separated by comma
    // input: [0., 1., 0.5]
    // output: "0.,1.,0.5"
    auto combine_doubles = [](std::string a, double b) -> std::string {
        return std::move(a) + "," + fmt::format("{}", b);
    };

    for (const auto& url: _urls) {
        if (verbose_) fmt::print(file_, "-> Checking {}\n", url);
        features_t features(url, _url_feature_flags, _html_feature_flags,
            _host_based_feature_flags, _label);
        features.set_verbose(verbose_);
        if (_html_feature_flags) {
            features.set_html_features_opts(std::string(_node_bin), std::string(_html_script));
        }
        auto fvec = features.compute_feature_vector();
        std::string line = std::accumulate(std::next(fvec.begin()), fvec.end(),
                                           fmt::format("{}", fvec[0]),
                                           combine_doubles);
        if (verbose_) {
            line += features.compute_extra_values();
        }
        fmt::print(file_, "{}\n", line);
    }
}
