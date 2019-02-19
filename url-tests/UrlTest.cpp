#include <algorithm>
#include <iostream>

#include "UrlTest.h"

UrlTest::UrlTest()
        :m_test_flags(Test::None)
{
}

void UrlTest::SetUrl(const std::string& url)
{
    // TODO: add some logger
    std::cout << "Setting up the URL: " << url << "\n";
    m_raw_url = url;
    try {
        m_url = Poco::URI(url);
    }
    catch (const Poco::Exception& ex) {
        std::cerr << ex.what() << ex.message() << "\n";
    }
}

int UrlTest::PerformtTests()
{
    std::cout << "Executing tests...\n";
    int result = 0;
    if (m_test_flags & Test::Length) {
        result += TestLength();
    }
    if (m_test_flags & Test::Depth) {
        result += TestDepth();
    }
    return result;
}

void UrlTest::AddTestLength(int length)
{
    std::cout << "Adding test for length (" << length << ").\n";
    m_test_flags |= Test::Length;
    m_test_url_length = length;
}

int UrlTest::TestLength()
{
    std::cout << "Testing URL length... ";
    if (m_raw_url.size()>m_test_url_length) {
        std::cout << "FAIL\n";
        return 1;
    }
    std::cout << "PASS\n";
    return 0;
}

void UrlTest::AddTestDepth(int depth)
{
    std::cout << "Adding test for depth (" << depth << ").\n";
    m_test_flags |= Test::Depth;
    m_test_url_depth = depth;
}

int UrlTest::TestDepth()
{
    std::cout << "Testing URL depth... ";
    const auto& host = m_url.getHost();
    auto dots = std::count_if(host.begin(), host.end(), [](const char& c) {
        return c=='.';
    });
    if (dots>m_test_url_depth) {
        std::cout << "FAIL\n";
        return 1;
    }
    std::cout << "PASS\n";
    return 0;
}




