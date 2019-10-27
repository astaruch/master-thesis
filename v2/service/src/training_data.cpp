#include "training_data.h"

// #include <algorithm>
#include <numeric>

// #include <spdlog/spdlog.h>

#include "features/feature_base.h"
#include "features/ip_address.h"
#include "features/url_length.h"


void training_data::set_feature_flags(uint64_t flags)
{
    _feature_flags = flags;
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

bool training_data::create_training_data()
{

    //fmt::print("-- file '{}':\n", _output_name);
    std::string csv_header = create_csv_header();
    //fmt::print("{}\n", csv_header);

    std::vector<std::string> lines = transform_urls_to_training_data();
    for (const std::string& line: lines) {
        //fmt::print("{}\n", line);
    }
    //fmt::print("--\n");

    return true;
}

std::string training_data::create_csv_header()
{
    std::vector<std::string> columns;

    if ((_feature_flags & feature_name::ip_address) == feature_name::ip_address) {
        columns.push_back(feature::ip_address::name());
    }
    if ((_feature_flags & feature_name::url_length) == feature_name::url_length) {
        columns.push_back(feature::url_length::name());
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
    // auto combine_doubles = [](std::string a, double b) -> std::string {
    //     return std::move(a) + "," + fmt::format("{}", b);
    // };

    std::vector<std::string> lines;
    // for (const auto& url: _urls) {
    //     auto fvec = compute_feature_vector(url);
    //     std::string line = std::accumulate(std::next(fvec.begin()), fvec.end(),
    //                                        fmt::format("{}", fvec[0]),
    //                                        combine_doubles);
    //     lines.push_back(line);
    //     _training_data.push_back(std::move(fvec));
    // }
    return lines;
}

std::vector<double> training_data::compute_feature_vector(const std::string& url)
{
    std::vector<double> fvec; // feature vector
    if ((_feature_flags & feature_name::ip_address) == feature_name::ip_address) {
        feature::ip_address f;
        f.set_url(url);
        fvec.push_back(f.compute_value());
    }

    if ((_feature_flags & feature_name::url_length) == feature_name::url_length) {
        feature::url_length f;
        f.set_url(url);
        fvec.push_back(f.compute_value());
    }

    fvec.push_back(_label);
    return fvec;
}
