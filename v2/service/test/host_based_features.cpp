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
    EXPECT_EQ(0, f.compute_value_dns_a_record(true));
}

TEST(DNS, ArecordMissing)
{
    host_based_features_t f("https://aenbauk.com");
    EXPECT_EQ(1, f.compute_value_dns_a_record(true));
}


TEST(DNS, DNSSECOK)
{
    host_based_features_t f("https://nic.cz");
    EXPECT_EQ(0, f.compute_value_dnssec(true));
}

TEST(DNS, DNSSECmissing)
{
    host_based_features_t f("https://staruch.sk");
    EXPECT_EQ(1, f.compute_value_dnssec(true));
}

TEST(DNSRegex, ExtractDate1)
{
    host_based_features_t f("http://google.ro");
    auto date = f.get_dns_created(true);
    EXPECT_EQ("2000-07-17", date);
}

TEST(DNSRegex, ExtractDate2)
{
    host_based_features_t f("http://staruch.sk");
    auto date = f.get_dns_created(true);
    EXPECT_EQ("2018-09-26", date);
}

TEST(DNSRegex, ExtractDate3)
{
    host_based_features_t f("http://google.com");
    auto date = f.get_dns_created(true);
    EXPECT_EQ("1997-09-15", date);
}

TEST(DNSRegex, ExtractDate4)
{
    host_based_features_t f("http://google.cn");
    auto date = f.get_dns_created(true);
    EXPECT_EQ("2003-03-17", date);
}

TEST(DNSRegex, ExtractDate5)
{
    host_based_features_t f("http://youtube.jp");
    auto date = f.get_dns_created(true);
    EXPECT_EQ("2007-06-12", date);
}

TEST(DNSRegex, ExtractDate6)
{
    host_based_features_t f("http://nic.cz");
    auto date = f.get_dns_created(true);
    EXPECT_EQ("1997-10-30", date);
}

TEST(DNSRegex, ExtractDate7)
{
    host_based_features_t f("http://google.com.br");
    auto date = f.get_dns_created(true);
    EXPECT_EQ("1999-05-18", date);
}

TEST(DNSRegex, ExtractDate8)
{
    host_based_features_t f("http://i-dont-exist.com.cz");
    auto date = f.get_dns_created(true);
    EXPECT_EQ("", date);
}

TEST(DNSRegex, ExtractDateUpadted9)
{
    host_based_features_t f("http://google.com");
    auto date = f.get_dns_updated(true);
    // TODO ALERT: this is not unit test, it will change over time
    EXPECT_EQ("2019-09-09", date);
}

TEST(DNSRegex, ExtractDateUpadted10)
{
    host_based_features_t f("http://google.com.br");
    auto date = f.get_dns_updated(true);
    // TODO ALERT: this is not unit test, it will change over time
    EXPECT_EQ("2019-04-17", date);
}

TEST(DNSRegex, ExtractDateUpadted11)
{
    host_based_features_t f("http://nic.cz");
    auto date = f.get_dns_updated(true);
    // TODO ALERT: this is not unit test, it will change over time
    EXPECT_EQ("2016-11-22", date);
}

TEST(DNSRegex, ExtractDateUpadted12)
{
    host_based_features_t f("http://nic.sk");
    auto date = f.get_dns_updated(true);
    // TODO ALERT: this is not unit test, it will change over time
    EXPECT_EQ("2019-10-14", date);
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

TEST(Headers, X_FRAME_OK_DENY)
{
    host_based_features_t f("https://github.com/");
    EXPECT_EQ(0, f.compute_value_x_frame(true));
}

TEST(Headers, X_FRAME_OK_SAMEORIGIN)
{
    host_based_features_t f("https://google.com/");
    EXPECT_EQ(0, f.compute_value_x_frame(true));
}

TEST(Headers, X_FRAME_MISSING)
{
    host_based_features_t f("https://paypal.com");
    EXPECT_EQ(1, f.compute_value_x_frame(true));
}

TEST(Headers, X_CONTENT_TYPE_OPTS_OK)
{
    host_based_features_t f("https://developer.mozilla.org");
    EXPECT_EQ(0, f.compute_value_x_content_type(true));
}

TEST(Headers, X_CONTENT_TYPE_OPTS_MISSING)
{
    host_based_features_t f("https://paypal.com");
    EXPECT_EQ(1, f.compute_value_x_content_type(true));
}

TEST(DomainSimilarity, SimilarDomain)
{
    host_based_features_t f("http://some.weird.domain.paypel.com");
    EXPECT_EQ(1, f.compute_similar_domain(true));
}

TEST(DomainSimilarity, SameDomain)
{
    host_based_features_t f("http://paypal.com");
    EXPECT_EQ(0, f.compute_similar_domain(true));
}


TEST(DomainSimilarity, Unknowndomain)
{
    host_based_features_t f("http://aiohsgoiashgoiashgoiahgoihaigo.com");
    EXPECT_EQ(1, f.compute_similar_domain(true));
}

TEST(DomainSimilarity, MultipleCharDifferent)
{
    host_based_features_t f("http://gaagle.com");
    EXPECT_EQ(0.5, f.compute_similar_domain(true));
}

TEST(ASN, GetASN)
{
    host_based_features_t f("http://google.com");
    f.compute_value_dns_a_record(true);
    EXPECT_EQ("AS15169", f.get_asn());
}
