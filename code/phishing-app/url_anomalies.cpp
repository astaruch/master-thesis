#include "url_anomalies.h"


int url_anomalies::check_length(size_t length) const {
    // TODO: find out what is a good value to return?
    return m_url.length() > length ? 1 : 0;
}

int url_anomalies::check_depth(size_t depth) const {
    return 0;
}

int url_anomalies::check_depth(size_t depth) {

    return 0;
}

bool url_anomalies::parser::parse(const std::string& uri) {
    m_uri = uri;

    return false;
}

bool url_anomalies::parser::parse_scheme(const std::string& uri) {
    m_scheme = "";
    for (size_t i = 0; i < uri.length(); ++i) {
        const char& c = uri[i];
        if (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z')) {
            continue;
        }

        if (('0' <= c && c <= '9') || c == '+' || c == '-' || c == '.') {
            if (i == 0) {
                return true;
            }
        }

        if (c == ':') {
            if (i == 0) {
                // TODO: error, missing protocol scheme
                return false;
            }
            m_scheme = uri.substr(0, i);
            return true;
        }
        // we have encountered an invalid character,
        // so there is no valid scheme
        return true;
    }
    return true;
}
