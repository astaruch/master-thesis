#include "features.h"

#include "url_features.h"
#include "host_based_features.h"
#include "html_features.h"

#include <Poco/Exception.h>
#include <fmt/format.h>

features_t::features_t(const std::string& url, bool verbose = false)
    : features_t(url, 0, 0, 0, 0, verbose, false)
{
}

features_t::features_t(const std::string& url, uint64_t url_flags, uint64_t html_flags,
                       uint64_t host_based_flags, int label, bool verbose = false,
                       bool extra_values = false)
    : _url(url)
    , _url_flags(url_flags)
    , _html_flags(html_flags)
    , _host_based_flags(host_based_flags)
    , _label(label)
    , _url_is_ok(true)
    , verbose_(verbose)
    , extra_values_flag_(extra_values)
{
    try {
        _parsed = Poco::URI(url);
    } catch (const Poco::SyntaxException& ex) {
        if (verbose_) fmt::print(stderr, "{}: {}\n", ex.what(), ex.message());
        _url_is_ok = false;
    }
}

std::vector<double> features_t::compute_feature_vector()
{
    std::vector<double> fvec;

    if (_url_flags) {
        url_features_t url_features(_url, _parsed, _url_flags, _url_is_ok);
        auto values = url_features.compute_values_vec();
        fvec.insert(fvec.end(), values.begin(), values.end());
    }

    if (_html_flags) {
        std::vector<double> values;
        if (!htmlfeatures_bin_.empty()) {
            html_features_t html_features(_url, _html_flags, htmlfeatures_bin_);
            values = html_features.get_values_from_external_script();
        } else {
            html_features_t html_features(_url, _html_flags, node_bin_, html_script_);
            values = html_features.compute_values();
        }
        fvec.insert(fvec.end(), values.begin(), values.end());
    }

    if (_host_based_flags) {
        host_based_features_t host_based_features(_url, _parsed, _host_based_flags, _url_is_ok);
        auto values = host_based_features.compute_values_vec();
        fvec.insert(fvec.end(), values.begin(), values.end());

        extra_values_ += host_based_features.extra_values();
    }

    fvec.push_back(_label);

    return fvec;
}

std::string features_t::compute_extra_values() const
{
    return extra_values_;
}

void features_t::set_html_features_opts(const std::string& node_bin, const std::string& html_script,
    const std::string& htmlfeatures_bin)
{
    node_bin_ = node_bin;
    html_script_ = html_script;
    htmlfeatures_bin_ = htmlfeatures_bin;
}
