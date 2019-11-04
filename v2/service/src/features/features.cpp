#include "features.h"

#include <Poco/Exception.h>
#include <fmt/format.h>

void features_t::set_url(const std::string& url)
{
    _url = url;
    try {
        _url_obj = Poco::URI(url);
        _url_is_ok = true;
    } catch (const Poco::SyntaxException& ex) {
        fmt::print(stderr, "Error. URL '{}' is malformed.\n", url);
        _url_obj = Poco::URI();
        _value = 1.0; // we say that URL is phishing if we can't parse it
    }
}

