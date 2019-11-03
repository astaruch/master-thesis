#include "training_data.h"

#include <algorithm>
#include <numeric>

#include <spdlog/spdlog.h>

#include "features/url_features.h"
#include "features/feature_base.h"
#include "features/all_features.h"


void training_data::set_feature_flags(uint64_t flags)
{
    _feature_flags = flags;
}

void training_data::set_html_feature_flags(uint64_t flags)
{
    _html_feature_flags = flags;
}

void training_data::set_input_data(std::vector<std::string> urls)
{
    _urls.swap(urls);
}

void training_data::set_label(double label)
{
    _label = label;
}

void training_data::set_output_name(std::string name)
{
    _output_name = name;
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

    fmt::print("-- file '{}':\n", _output_name);
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
        if (_feature_flags & id) {
            columns.push_back(std::string(feature::base::column_names.at(id)));
        }
    }

    for (const auto id: feature_enum::html) {
        if (_html_feature_flags & id) {
            columns.push_back(std::string(feature::base::column_names.at(id)));
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
        auto fvec = compute_feature_vector(url);
        std::string line = std::accumulate(std::next(fvec.begin()), fvec.end(),
                                           fmt::format("{}", fvec[0]),
                                           combine_doubles);
        lines.push_back(line);
        _training_data.push_back(std::move(fvec));
    }
    return lines;
}

std::vector<double> training_data::compute_feature_vector(const std::string& url)
{
    std::vector<double> fvec; // feature vector
    for (const auto id: feature_enum::url) {
        if (_feature_flags & id) {
            auto f = feature::creator::create_feature_from_flag(id);
            f->set_url(url);
            fvec.push_back(f->compute_value());
        }
    }
    {
        auto html_feat = feature::html_features(_node_bin, _html_script, url, _html_feature_flags);
        // TODO: make more robust version with mapping column - value
        // we have computed values in same order like we have columns
        auto html_values = html_feat.compute_values();
        fvec.insert(fvec.end(), html_values.begin(), html_values.end());
    }

    fvec.push_back(_label);
    return fvec;
}
