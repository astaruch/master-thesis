#include "feature_base.h"

#include <Poco/Exception.h>
#include <fmt/format.h>

namespace feature {

const std::unordered_map<feature_enum::id, std::string_view> base::column_names = base::create_column_names();

std::unordered_map<feature_enum::id, std::string_view> base::create_column_names()
{
    std::unordered_map<feature_enum::id, std::string_view> map{
        {feature_enum::input_tag, "input_tag"}
    };
    return map;
}

void base::set_url(const std::string& url)
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

double base::normalize_value(int min, int value, int max)
{
    value = std::max(min, std::min(value, max));
    return static_cast<double>(value - min) / static_cast<double>(max - min);
}

} // namespace feature

