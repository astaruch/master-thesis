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
        IpAddressOccurrence = 0x20,
        NonStandardPort = 0x40,
        NonStandardTLD = 0x80,
        ScriptInQuery = 0x100,
    };

    UrlTest();

    void SetUrl(const std::string& url); //TODO: add check when it throws

    void AddTestLength(int length);
    void AddTestDepth(int depth);
    void AddTestSpecialChars();
    void AddTestKeywords(const std::string& keywords);
    void AddTestEncodedChars();
    void AddTestIpAddressOccurrence();
    void AddTestNonStandardPort();
    void AddTestNonStandardTLD();
    void AddTestScriptsInQuery();

    int PerformTests();

private:
    Poco::URI m_url; //!< Parsed URL object
    std::string m_raw_url; //!< Raw URL
    std::string m_tld; //!< Top level domain
    std::string m_sld; //!< Second level domain
    //! Labels (or component) of hostname. If www.inf.ed.ac.uk, then m_tld = uk, m_labels[0] = ac, m_labels[1] = ed, ...
    std::vector<std::string> m_components;

    int m_test_url_length;
    int m_test_url_depth;
    std::vector<std::string> m_test_keywords;

    unsigned m_test_flags;

    int TestLength();
    int TestDepth();
    int TestSpecialChars();
    int TestKeywords();
    int TestEncodedChars();
    int TestIpAddressOccurrence();
    int TestNonStandardPort();
    int TestNonStandardTLD();
    int TestScriptsInQuery();

    int TestPunyCode();
    int TestPercentEncoding();
};

#endif //URL_TESTS_URLTEST_H
