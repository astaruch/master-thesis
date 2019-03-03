#include <algorithm>
#include <iostream>

#include <Poco/StringTokenizer.h>

#include "UrlTest.h"

UrlTest::UrlTest()
        : m_test_flags(Test::None) {
}

void
UrlTest::SetUrl(const std::string &url) {
    // TODO: add some logger
    std::cout << "Setting up the URL: " << url << "\n";
    m_raw_url = url;
    try {
        m_url = Poco::URI(url);
    }
    catch (const Poco::Exception &ex) {
        std::cerr << ex.what() << ex.message() << "\n";
    }
}

int
UrlTest::PerformtTests() {
    std::cout << "Executing tests...\n";
    int result = 0;
    if (m_test_flags & Test::Length) {
        result += TestLength();
    }
    if (m_test_flags & Test::Depth) {
        result += TestDepth();
    }
    if (m_test_flags & Test::SpecialChars) {
        result += TestSpecialChars();
    }
    if (m_test_flags & Test::Keywords) {
        result += TestKeywords();
    }
    return result;
}

void
UrlTest::AddTestLength(int length) {
    std::cout << "Adding test for length (" << length << ").\n";
    m_test_flags |= Test::Length;
    m_test_url_length = length;
}

int
UrlTest::TestLength() {
    std::cout << "Testing URL length... ";
    if (m_raw_url.size() > m_test_url_length) {
        std::cout << "FAIL\n";
        return 1;
    }
    std::cout << "PASS\n";
    return 0;
}

void
UrlTest::AddTestDepth(int depth) {
    std::cout << "Adding test for depth (" << depth << ").\n";
    m_test_flags |= Test::Depth;
    m_test_url_depth = depth;
}

int
UrlTest::TestDepth() {
    std::cout << "Testing URL depth... ";
    const auto &host = m_url.getHost();
    auto dots = std::count_if(host.begin(), host.end(), [](const char &c) {
        return c == '.';
    });
    if (dots > m_test_url_depth) {
        std::cout << "FAIL\n";
        return 1;
    }
    std::cout << "PASS\n";
    return 0;
}

void
UrlTest::AddTestSpecialChars() {
    std::cout << "Adding test for special characters.\n";
    m_test_flags |= Test::SpecialChars;
}

int
UrlTest::TestSpecialChars() {
    int result = 0;
    std::cout << "Testing special chars in URL... \n";

    std::cout << "@ (user@password) ... ";
    if (!m_url.getUserInfo().empty()) {
        // @
        result += 1;
        std::cout << "YES\n";
    } else {
        std::cout << "NO\n";
    }

    // TODO: determine correct values for these special chars
    // TODO: test maybe only www-google-com.info or something like that with special chars
    std::vector<char> special_chars{'@', '/', '?', '.', '=', '-', '_'};
    for (const auto &c: special_chars) {
        std::cout << c << " ... ";
        if (std::count(m_raw_url.begin(), m_raw_url.end(), c) > 0) {
            result += 1; // TODO: change to something meaningful
            std::cout << "YES\n";
        } else {
            std::cout << "NO\n";
        }
    }
    std::cout << (result ? "FAIL" : "PASS") << std::endl;
    return result;
}

void
UrlTest::AddTestKeywords(const std::string &keywords) {
    std::cout << "Adding test for keywords.\n";
    m_test_flags |= Test::Keywords;

    auto tokens = Poco::StringTokenizer(keywords, ",");
    m_test_keywords = std::vector<std::string>(tokens.begin(), tokens.end());
}

int
UrlTest::TestKeywords() {
    std::cout << "Testing keywords in URL... ";
    int result = 0;
    for (const auto &keyword: m_test_keywords) {
        if (m_raw_url.find(keyword) != std::string::npos) {
            result += 1;
        }
    }
    std::cout << (result ? "FAIL" : "PASS") << std::endl;
    return result;
}

