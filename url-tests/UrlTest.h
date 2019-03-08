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
        SpecialChars = 0x4,
        Keywords = 0x8,
        EncodedChars = 0x10,
    };

    UrlTest();

    void SetUrl(const std::string& url); //TODO: add check when it throws

    void AddTestLength(int length);
    void AddTestDepth(int depth);
    void AddTestSpecialChars();
    void AddTestKeywords(const std::string& keywords);
    void AddTestEncodedChars();

    int PerformtTests();

private:
    Poco::URI m_url;
    std::string m_raw_url;

    int m_test_url_length;
    int m_test_url_depth;
    std::vector<std::string> m_test_keywords;

    unsigned m_test_flags;

    int TestLength();
    int TestDepth();
    int TestSpecialChars();
    int TestKeywords();
    int TestEncodedChars();
};

#endif //URL_TESTS_URLTEST_H
