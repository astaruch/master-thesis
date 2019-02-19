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

void UrlTest::AddTestLength(int length)
{
    std::cout << "Adding test for length (" << length << ").\n";
    m_test_flags |= Test::Length;
    m_test_url_length = length;
}

int UrlTest::PerformtTests()
{
    std::cout << "Executing tests...\n";
    int result = 0;
    if (m_test_flags & Test::Length) {
        result += TestLength();
    }
    return result;
}

int UrlTest::TestLength()
{
    std::cout << "Testing URL length...\n";
    if (m_raw_url.size() > m_test_url_length)
    {
        return 1;
    }
    return 0;
}




