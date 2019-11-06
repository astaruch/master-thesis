#include "training_data.h"

#include "features/feature_enum.h"
#include "features/features.h"
#include "features/url_features.h"
#include "features/html_features.h"

#include <algorithm>
#include <numeric>

#include <spdlog/spdlog.h>

void training_data::set_flags(uint64_t all, uint64_t url, uint64_t html, uint64_t host_based)
{
    _feature_flags = all;
    _url_feature_flags = url;
    _html_feature_flags = html;
    _host_based_feature_flags = host_based;
}

void training_data::set_output(bool use_stdout, const std::string& output_name)
{
    use_stdout_ = use_stdout;
    output_name_ = output_name;
}

void training_data::set_input_data(std::vector<std::string> urls)
{
    _urls.swap(urls);
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
    fmt::print("-- file '{}':\n", output_name_);
    std::string csv_header = create_csv_header();
    fmt::print("{}\n", csv_header);

    std::vector<std::string> lines = transform_urls_to_training_data();
    for (const std::string& line: lines) {
        fmt::print("{}\n", line);
    }
    fmt::print("--\n");

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

    return std::accumulate(columns.begin(), columns.end(), std::string(),
        [](const std::string& a, const std::string& b) -> std::string {
            return a + (a.length() > 0 ? "," : "") + b;
        }
    );
}

std::vector<std::string> training_data::transform_urls_to_training_data()
{
    // takes vector of doubles and transform it to the string separated by comma
    // input: [0., 1., 0.5]
    // output: "0.,1.,0.5"
    auto combine_doubles = [](std::string a, double b) -> std::string {
        return std::move(a) + "," + fmt::format("{}", b);
    };

    std::vector<std::string> lines;
    for (const auto& url: _urls) {
        features_t features(url, _url_feature_flags, _html_feature_flags,
            _host_based_feature_flags, _label);
        if (_html_feature_flags) {
            features.set_html_features_opts(std::string(_node_bin), std::string(_html_script));
        }
        auto fvec = features.compute_feature_vector();
        std::string line = std::accumulate(std::next(fvec.begin()), fvec.end(),
                                           fmt::format("{}", fvec[0]),
                                           combine_doubles);
        lines.push_back(line);
        _training_data.push_back(std::move(fvec));
    }
    return lines;
}
