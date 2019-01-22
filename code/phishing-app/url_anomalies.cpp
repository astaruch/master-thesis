#include <algorithm>
#include <iostream>

#include "url_anomalies.h"
#include "third-party/lurlparser/LUrlParser.h"

void url_anomalies::parse_url(const std::string& url) {
    m_url = url;
    m_parsed_url = LUrlParser::clParseURL::ParseURL(url);
}

int url_anomalies::check_length(size_t allowed_length) const {
    // TODO: find out what is an optimal value to return
    bool anomaly = m_url.length() > allowed_length;
    if (anomaly) {
        std::cout << "|length|";
        return 1;
    }
    return 0;
}

int url_anomalies::check_depth(size_t allowed_dots) const {
    const auto& host = m_parsed_url.m_Host;
    auto dots = std::count_if(host.begin(), host.end(), [](const char& c) {
        return c == '.';
    });
    // TODO: find out what is an optimal value to return
    bool anomaly = dots >= allowed_dots;
    if (anomaly) {
        std::cout << "|depth|";
        return 1;
    }
    return 0;
}

int url_anomalies::check_keywords() const {
    for (const auto& keyword: keywords) {
        if (m_url.find(keyword) != std::string::npos) {
            std::cout << "|keywords|";
            return 1; // TODO: find out what is an optimal value to return
        }
    }
    return 0;
}

int url_anomalies::perform_tests(size_t allowed_length, size_t allowed_depth) const {
    return check_length(allowed_length) + check_depth(allowed_depth) + check_keywords();
}

