#include "training_data.h"

#include "features/feature_enum.h"
#include "features/url_features.h"
#include "features/html_features.h"
#include "features/host_based_features.h"

#include <algorithm>
#include <numeric>

#include <spdlog/spdlog.h>
#include <Poco/URI.h>
#include <Poco/Exception.h>

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
void training_data::set_html_features_opts(std::string_view node_bin, std::string_view html_script,
    std::string_view htmlfeatures_bin)
{
    node_bin_ = node_bin;
    html_script_ = html_script;
    htmlfeatures_bin_ = htmlfeatures_bin;
}

bool training_data::create_training_data()
{

    std::string csv_header = output_include_url_ ? "url," : "";
    csv_header += create_csv_header();
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

    // for (const auto id: feature_enum::html) {
    //     if (_html_feature_flags & id) {
    //         columns.push_back(std::string(feature_enum::column_names.at(id)));
    //     }
    // }
    if (_html_feature_flags) {
        html_features_t html("", _html_feature_flags, htmlfeatures_bin_, output_extra_values_);
        auto html_header = html.get_header();
        fmt::print("HEADER = {}\n", html_header);
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
        if (_host_based_feature_flags & id) {
            columns.push_back(std::string(feature_enum::column_names.at(id)));
        }
    }

    columns.push_back("label");

    if (output_extra_values_) {
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
        if (verbose_) fmt::print(file_, "--> Checking {}\n", url);
        try {
            const Poco::URI parsed = Poco::URI(url); // throws SyntaxException on bad URL
            std::vector<double> fvec; // feature vector for an URL

            if (_url_feature_flags) {
                url_features_t url_features(url, parsed, _url_feature_flags, true);
                auto values = url_features.compute_values_vec();
                fvec.insert(fvec.end(), values.begin(), values.end());
            }

            if (_html_feature_flags) {
                std::vector<double> values;
                if (!htmlfeatures_bin_.empty()) {
                    html_features_t html_features(url, _html_feature_flags, htmlfeatures_bin_, output_extra_values_);
                    values = html_features.get_values_from_external_script();
                } else {
                    html_features_t html_features(url, _html_feature_flags, node_bin_, html_script_);
                    values = html_features.compute_values();
                }
                fvec.insert(fvec.end(), values.begin(), values.end());
            }

            std::string host_extra_values = "";
            if (_host_based_feature_flags) {
                host_based_features_t host_based_features(url, parsed, _host_based_feature_flags, true);
                auto values = host_based_features.compute_values_vec();
                fvec.insert(fvec.end(), values.begin(), values.end());

                if (output_extra_values_) {
                    host_extra_values = ',' + host_based_features.extra_values();
                }
            }
            std::string line;

            line += std::accumulate(std::next(fvec.begin()), fvec.end(),
                                            fmt::format("{}", fvec[0]),
                                            combine_doubles);
            fmt::print(file_, "{}{}{}\n",
                (output_include_url_ ? fmt::format("\"{}\",", url) : ""),
                line,
                host_extra_values);

        } catch (const Poco::SyntaxException& ex) {
            if (verbose_) fmt::print(stderr, "{}: {}\n", ex.what(), ex.message());
            continue;
        }
    }
}
