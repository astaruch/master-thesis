#include "features.h"

#include "url_features.h"
#include "host_based_features.h"
#include "html_features.h"

#include <Poco/Exception.h>
#include <fmt/format.h>

features_t::features_t(const std::string& url)
try
    : _url(url)
    , _url_flags(0)
    , _html_flags(0)
    , _host_based_flags(0)
    , _label(0)
    , _parsed(Poco::URI(url))
    , _url_is_ok(true)
{
}
catch (const Poco::SyntaxException& ex)
{
    _parsed = Poco::URI();
    _url_is_ok = false;
}

features_t::features_t(const std::string& url, uint64_t url_flags, uint64_t html_flags,
                       uint64_t host_based_flags, int label)
try
    : _url(url)
    , _url_flags(url_flags)
    , _html_flags(html_flags)
    , _host_based_flags(host_based_flags)
    , _label(label)
    , _parsed(Poco::URI(url))
    , _url_is_ok(true)
{
}
catch (const Poco::SyntaxException& ex)
{
    _parsed = Poco::URI();
    _url_is_ok = false;
}

std::vector<double> features_t::compute_feature_vector() const
{
    std::vector<double> fvec;

    if (_url_flags) {
        url_features_t url_features(_url, _parsed, _url_flags, _url_is_ok);
        auto values = url_features.compute_values_vec();
        fvec.insert(fvec.end(), values.begin(), values.end());
    }

    if (_html_flags) {
        // TODO: add constructor to accept executable path (omit NodeJS + script)
        html_features_t html_features(_url, _html_flags, _node_bin, _html_script);
        auto values = html_features.compute_values();
        fvec.insert(fvec.end(), values.begin(), values.end());
    }

    if (_host_based_flags) {
        host_based_features_t host_based_features(_url, _parsed, _host_based_flags, _url_is_ok);
        auto values = host_based_features.compute_values_vec();
        fvec.insert(fvec.end(), values.begin(), values.end());
    }

    fvec.push_back(_label);

    return fvec;
}

void features_t::set_html_features_opts(const std::string& node_bin, const std::string& html_script)
{
    _node_bin = node_bin;
    _html_script = html_script;
}
