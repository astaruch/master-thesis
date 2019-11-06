#include "gtest/gtest.h"
#include "features/host_based_features.h"

TEST(Redirect, 301)
{
    host_based_features_t f("http://google.com");
    EXPECT_EQ(1., f.compute_value_redirect());
}

TEST(Redirect, 200)
{
    host_based_features_t f("https://www.google.com");
    EXPECT_EQ(0, f.compute_value_redirect());
}

TEST(GoogleIndex, Indexed)
{
    host_based_features_t f("https://www.google.com");
    EXPECT_EQ(0, f.compute_value_google_indexed());
}

TEST(GoogleIndex, NotIndexed)
{
    host_based_features_t f("http://www.acceso24.banorte.com.bxi058.xyz");
    EXPECT_EQ(1, f.compute_value_google_indexed());
}

TEST(DNS, ArecordOK)
{
    host_based_features_t f("http://google.cz");
    EXPECT_EQ(0, f.compute_value_dns_a_record());
}

TEST(DNS, ArecordMissing)
{
    host_based_features_t f("https://aenbauk.com");
    EXPECT_EQ(1, f.compute_value_dns_a_record());
}


TEST(DNS, DNSSECOK)
{
    host_based_features_t f("https://nic.cz");
    EXPECT_EQ(0, f.compute_value_dnssec());
}

TEST(DNS, DNSSECmissing)
{
    host_based_features_t f("https://staruch.sk");
    EXPECT_EQ(1, f.compute_value_dnssec());
}

TEST(DNSRegex, ExtractDate1)
{
    host_based_features_t f("http://google.ro");
    auto date = f.extract_dns_date(true);
    EXPECT_EQ("20000717", date);
}

TEST(DNSRegex, ExtractDate2)
{
    host_based_features_t f("http://staruch.sk");
    auto date = f.extract_dns_date(true);
    EXPECT_EQ("20180926", date);
}

TEST(DNSRegex, ExtractDate3)
{
    host_based_features_t f("http://google.com");
    auto date = f.extract_dns_date(true);
    EXPECT_EQ("19970915", date);
}

TEST(DNSRegex, ExtractDate4)
{
    host_based_features_t f("http://google.cn");
    auto date = f.extract_dns_date(true);
    EXPECT_EQ("20030317", date);
}

TEST(DNSRegex, ExtractDate5)
{
    host_based_features_t f("http://youtube.jp");
    auto date = f.extract_dns_date(true);
    EXPECT_EQ("20070612", date);
}

TEST(DNSRegex, ExtractDate6)
{
    host_based_features_t f("http://nic.cz");
    auto date = f.extract_dns_date(true);
    EXPECT_EQ("19971030", date);
}

TEST(DNSRegex, ExtractDate7)
{
    host_based_features_t f("http://google.com.br");
    auto date = f.extract_dns_date(true);
    EXPECT_EQ("19990518", date);
}

TEST(DNSRegex, ExtractDate8)
{
    host_based_features_t f("http://i-dont-exist.com.cz");
    auto date = f.extract_dns_date(true);
    EXPECT_EQ("", date);
}

TEST(SSL, SubjectMatchHostname)
{
    host_based_features_t f("https://www.google.com");
    EXPECT_EQ(0, f.compute_value_ssl_subject(true));
}

TEST(SSL, SubjectDoesntMatchHostname)
{
    host_based_features_t f("https://google.com");
    EXPECT_EQ(1, f.compute_value_ssl_subject(true));
}

TEST(SSL, NoCert)
{
    host_based_features_t f("http://google.com");
    EXPECT_EQ(1, f.compute_value_ssl_subject(true));
}

TEST(Headers, HSTS_false)
{
    host_based_features_t f("http://google.com");
    EXPECT_EQ(1, f.compute_value_hsts(true));
}

TEST(Headers, HSTS_OK)
{
    host_based_features_t f("https://content-security-policy.com");
    EXPECT_EQ(0, f.compute_value_hsts(true));
}

TEST(Headers, HSTS_invalid_url)
{
    host_based_features_t f("http://goog1231231231le.com");
    EXPECT_EQ(1, f.compute_value_hsts(true));
}

TEST(Headers, XSS_OK)
{
    host_based_features_t f("https://github.com");
    EXPECT_EQ(0, f.compute_value_xss_protection(true));
}

TEST(Headers, XSS_MISSING)
{
    host_based_features_t f("https://google.com");
    EXPECT_EQ(1, f.compute_value_xss_protection(true));
}

TEST(Headers, CSP_OK)
{
    host_based_features_t f("https://content-security-policy.com/presentations/");
    EXPECT_EQ(0, f.compute_value_csp(true));
}

TEST(Headers, CSP_MISSING)
{
    host_based_features_t f("https://google.sk");
    EXPECT_EQ(1, f.compute_value_csp(true));
}
