#ifndef URL_TESTS_URLTEST_H
#define URL_TESTS_URLTEST_H

#include <Poco/URI.h>
#include <Poco/Exception.h>

class UrlTest {
public:
    enum Test: unsigned {
        None = 0x0,
        Length = 0x1,
        Depth = 0x2,
    };

    UrlTest();

    void SetUrl(const std::string& url); //TODO: add check when it throws

    void AddTestLength(int length);
    void AddTestDepth(int depth);

    int PerformtTests();

private:
    Poco::URI m_url;
    std::string m_raw_url;

    int m_test_url_length;
    int m_test_url_depth;

    unsigned m_test_flags;

    int TestLength();
    int TestDepth();
};

#endif //URL_TESTS_URLTEST_H
