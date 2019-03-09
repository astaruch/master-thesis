#include <algorithm>
#include <iostream>

#include <Poco/StringTokenizer.h>
#include <Poco/Net/IPAddress.h>

#include "UrlTest.h"

UrlTest::UrlTest()
    : m_test_flags(Test::None)
{
}

void UrlTest::SetUrl(const std::string& url)
{
    // TODO: add some logger
    std::cout << "Setting up the URL: " << url << "\n";
    m_raw_url = url;
    try
    {
        m_url = Poco::URI(url);
    }
    catch (const Poco::Exception& ex)
    {
        std::cerr << ex.what() << ex.message() << "\n";
    }

    auto labels = Poco::StringTokenizer(m_url.getHost(), ".");
    m_components = std::vector<std::string>(labels.begin(), labels.end());
    m_tld = m_components.back();
    m_components.pop_back();
    std::reverse(m_components.begin(), m_components.end()); // reverse it so we have most relevant domain on [0] pos
    m_sld = m_components.front(); // have a shortcut to second level domain
}

int UrlTest::PerformTests()
{
    std::cout << "Executing tests...\n";
    int result = 0;
    if (m_test_flags & Test::Length)
    {
        result += TestLength();
    }
    if (m_test_flags & Test::Depth)
    {
        result += TestDepth();
    }
    if (m_test_flags & Test::SpecialChars)
    {
        result += TestSpecialChars();
    }
    if (m_test_flags & Test::Keywords)
    {
        result += TestKeywords();
    }
    if (m_test_flags & Test::SpecialChars)
    {
        result += TestEncodedChars();
    }
    if (m_test_flags & Test::IpAddressOccurence)
    {
        result += TestIpAddressOccurrence();
    }
    if (m_test_flags & Test::NonStandardPort)
    {
        result += TestNonStandardPort();
    }
    if (m_test_flags & Test::NonStandardTLD)
    {
        result += TestNonStandardTLD();
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
    if (m_raw_url.size() > m_test_url_length)
    {
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
        return c == '.';
    });
    if (dots > m_test_url_depth)
    {
        std::cout << "FAIL\n";
        return 1;
    }
    std::cout << "PASS\n";
    return 0;
}

void UrlTest::AddTestSpecialChars()
{
    std::cout << "Adding test for special characters.\n";
    m_test_flags |= Test::SpecialChars;
}

int UrlTest::TestSpecialChars()
{
    int result = 0;
    std::cout << "Testing special chars in URL... \n";

    std::cout << "@ (user@password) ... ";
    if (!m_url.getUserInfo().empty())
    {
        // @
        result += 1;
        std::cout << "YES\n";
    } else
    {
        std::cout << "NO\n";
    }

    // TODO: determine correct values for these special chars
    // TODO: test maybe only www-google-com.info or something like that with special chars
    std::vector<char> special_chars{'@', '/', '?', '.', '=', '-', '_'};
    for (const auto& c: special_chars)
    {
        std::cout << c << " ... ";
        if (std::count(m_raw_url.begin(), m_raw_url.end(), c) > 0)
        {
            result += 1; // TODO: change to something meaningful
            std::cout << "YES\n";
        } else
        {
            std::cout << "NO\n";
        }
    }
    std::cout << (result ? "FAIL" : "PASS") << std::endl;
    return result;
}

void UrlTest::AddTestKeywords(const std::string& keywords)
{
    std::cout << "Adding test for keywords.\n";
    m_test_flags |= Test::Keywords;

    auto tokens = Poco::StringTokenizer(keywords, ",");
    m_test_keywords = std::vector<std::string>(tokens.begin(), tokens.end());
}

int UrlTest::TestKeywords()
{
    std::cout << "Testing keywords in URL... ";
    int result = 0;
    for (const auto& keyword: m_test_keywords)
    {
        if (m_raw_url.find(keyword) != std::string::npos)
        {
            result += 1;
        }
    }
    std::cout << (result ? "FAIL" : "PASS") << std::endl;
    return result;
}

void UrlTest::AddTestEncodedChars()
{
    std::cout << "Adding test for encoded characters.\n";
    m_test_flags |= Test::EncodedChars;
}

int UrlTest::TestPunyCode()
{
    // Internationalize Domain Names in Applications
    // The xn-- says "everything that follows is encoded-unicode".
    long result = std::count_if(m_components.begin(), m_components.end(), [](const std::string& component) {
        std::string decoded;
        Poco::URI::decode(component, decoded);
        return decoded.size() >= 4 && decoded.substr(0, 4) == "xn--";
    });

    return result; //TODO: change return type
}

int UrlTest::TestPercentEncoding()
{
    // count all encoded chars in host in form of '%xx' where xx is one byte used hexadecimal
    int result = 0;
    size_t pos = 0;
    const auto& host = m_url.getHost();
    while (pos != std::string::npos)
    {
        pos = host.find('%', pos);
        // ensure that % is followed by at least two chars
        if (pos == std::string::npos || pos + 2 > host.size())
        {
            break;
        }
        if (std::isxdigit(host[pos + 1]) && std::isxdigit(host[pos + 2]))
        {
            ++result;
        }
        ++pos;
    }

    return result;
}

int UrlTest::TestEncodedChars()
{
    std::cout << "Testing encoded characters... ";
    int result = TestPercentEncoding() + TestPunyCode();
    std::cout << (result ? "FAIL" : "PASS") << std::endl;
    return result;
}

void UrlTest::AddTestIpAddressOccurrence()
{
    std::cout << "Adding test for IP address occurrence.\n";
    m_test_flags |= Test::IpAddressOccurence;
}

int UrlTest::TestIpAddressOccurrence()
{
    // TODO: combine that IP address matches hostname resolving
    Poco::Net::IPAddress ip;
    return Poco::Net::IPAddress::tryParse(m_sld, ip) ? 1 : 0;
}

void UrlTest::AddTestNonStandardPort()
{
    std::cout << "Adding test for non standard port.\n";
    m_test_flags |= Test::NonStandardPort;
}

int UrlTest::TestNonStandardPort()
{
    if (m_url.getScheme() == "http")
    {
        return m_url.getPort() == 80 ? 1 : 0;
    }
    if (m_url.getScheme() == "https")
    {
        return m_url.getPort() == 443 ? 1 : 0;
    }
    return 0;
}

void UrlTest::AddTestNonStandardTLD()
{
    std::cout << "Adding test for non standard TLD.\n";
    m_test_flags |= Test::NonStandardTLD;
}

int UrlTest::TestNonStandardTLD()
{
    /*
     * Tato metoda je použitelná hlavně v kombinaci s metodou analýzy obsahu či metodou analýzy
existence a chování. Nalezneme v URL TLD a snažíme se odvodit, zda není příliš nepravděpodobné,
že by uživatel navštívil stránku s touto TLD (např. vůči historii dříve navštívených stránek), nebo že
by se stránka s daným obsahem (převážně pak jazykem) nacházela na takovéto TLD. Ke správnému
nastavení rozhodovacího algoritmu může být užitečná Tabulka 4, která zobrazuje deset nejčastěji
používaných TLD ve phishingových stránkách pro rok 2013

     */
    return 0;
}