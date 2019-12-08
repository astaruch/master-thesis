#include "feature_vector.h"

#include "features/feature_enum.h"
#include "features/url_features.h"
#include "features/html_features.h"
#include "features/host_based_features.h"

#include <fmt/format.h>
#include <Poco/Exception.h>
#include <Poco/URI.h>

namespace phishscore {

feature_vector::feature_vector(const options& opts, const std::string& url)
    : opts_(opts)
    , url_(url)
{
}

void feature_vector::print_in_csv_format()
{
    try {
        if (opts_.verbose) fmt::print("--> Checking {}\n", url_);
        const Poco::URI parsed = Poco::URI(url_); // throws SyntaxException on bad URL
        std::vector<double> fvec;

        if (opts_.flags.url) {
            url_features_t url_features(url_, parsed, opts_.flags.url, true);
            auto values = url_features.compute_values_vec();
            fvec.insert(fvec.end(), values.begin(), values.end());
        }

        if (opts_.flags.html) {
            html_features_t html_features(url_, opts_.flags.html, opts_.html_analysis.bin_path, opts_.fvec.extra_values);
            auto values = html_features.get_values_from_external_script();
            fvec.insert(fvec.end(), values.begin(), values.end());
        }

        std::string host_extra_values = "";
        if (opts_.flags.host_based) {
            host_based_features_t host_based_features(url_, parsed, opts_.flags.host_based);
            auto values = host_based_features.compute_values_vec();
            fvec.insert(fvec.end(), values.begin(), values.end());

            if (opts_.fvec.extra_values) {
                host_extra_values = ',' + host_based_features.extra_values();
            }
        }
        // takes vector of doubles and transform it to the string separated by comma
        // input: [0., 1., 0.5]
        // output: "0.,1.,0.5"
        auto combine_doubles = [](std::string a, double b) -> std::string {
            return std::move(a) + "," + fmt::format("{}", b);
        };

        std::string line = std::accumulate(std::next(fvec.begin()), fvec.end(),
                                           fmt::format("{}", fvec[0]),
                                           combine_doubles);
        fmt::print("{}{}{},{}\n",
            (opts_.fvec.include_url ? fmt::format("\"{}\",", url_) : ""),
            line,
            host_extra_values,
            opts_.fvec.class_label);
    } catch (const Poco::SyntaxException& ex) {
        if (opts_.verbose) fmt::print(stderr, "--> {}: {}\n", ex.what(), ex.message());
    }
}

std::unordered_map<std::string_view, double> feature_vector::compute_values()
{
    try {
        if (opts_.verbose) fmt::print("--> Checking {}\n", url_);
        const Poco::URI parsed = Poco::URI(url_); // throws SyntaxException on bad URL
        std::unordered_map<std::string_view, double> fmap;

        if (opts_.flags.url) {
            url_features_t url_features(url_, parsed, opts_.flags.url, true);
            auto values = url_features.compute_values_map();
            fmap.merge(values);
        }

        if (opts_.flags.html) {
            html_features_t html_features(url_, opts_.flags.html, opts_.html_analysis.bin_path, opts_.html_analysis.port);
            auto values = html_features.compute_values_map();
            fmap.merge(values);
        }

        if (opts_.flags.host_based) {
            host_based_features_t host_based_features(url_, parsed, opts_.flags.host_based);
            auto values = host_based_features.compute_values_map();
            fmap.merge(values);
        }
        if (opts_.verbose) fmt::print("--> Computed {} features\n", fmap.size());
        return fmap;
    } catch (const Poco::SyntaxException& ex) {
        if (opts_.verbose) fmt::print(stderr, "--> {}: {}\n", ex.what(), ex.message());
        return {};
    }
}


} // namespace phishscore
