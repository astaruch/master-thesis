#include "gtest/gtest.h"
#include "features/host_based_features.h"
#include "network_responses.hpp"

TEST(Redirect, 301)
{
    host_based_features_t f;
    EXPECT_EQ(1., f.compute_value_redirect(curl_http_google));
}

TEST(Redirect, 200)
{
    host_based_features_t f;
    EXPECT_EQ(0, f.compute_value_redirect(curl_https_google));
}

TEST(GoogleIndex, Indexed)
{
    host_based_features_t f;
    EXPECT_EQ(0, f.compute_value_google_indexed(""));
}

TEST(GoogleIndex, NotIndexed)
{
    host_based_features_t f;
    EXPECT_EQ(1, f.compute_value_google_indexed("...did not match any documents..."));
}

TEST(DNS, ArecordOK)
{
    host_based_features_t f;
    EXPECT_EQ(0, f.compute_value_dns_a_record(dig_google));
}

TEST(DNS, ArecordMissing)
{
    host_based_features_t f;
    EXPECT_EQ(1, f.compute_value_dns_a_record(dig_something_else));
}

TEST(DNS, DNSSECOK)
{
    host_based_features_t f;
    EXPECT_EQ(0, f.compute_value_dnssec(dig_nic_cz));
}

TEST(DNS, DNSSECmissing)
{
    host_based_features_t f;
    EXPECT_EQ(1, f.compute_value_dnssec(dig_google));
}

TEST(DNSRegex, ExtractDate1)
{
    host_based_features_t f;
    auto date = f.get_dns_created(whois_google_ro);
    EXPECT_EQ("2000-07-17", date);
}

TEST(DNSRegex, ExtractDate2)
{
    host_based_features_t f;
    auto date = f.get_dns_created(whois_staruch_sk);
    EXPECT_EQ("2018-09-26", date);
}

TEST(DNSRegex, ExtractDate3)
{
    host_based_features_t f;
    auto date = f.get_dns_created(whois_google_com);
    EXPECT_EQ("1997-09-15", date);
}

TEST(DNSRegex, ExtractDate4)
{
    host_based_features_t f;
    auto date = f.get_dns_created(whois_google_cn);
    EXPECT_EQ("2003-03-17", date);
}

TEST(DNSRegex, ExtractDate5)
{
    host_based_features_t f;
    auto date = f.get_dns_created(whois_youtube_jp);
    EXPECT_EQ("2007-06-12", date);
}

TEST(DNSRegex, ExtractDate6)
{
    host_based_features_t f;
    auto date = f.get_dns_created(whois_nic_cz);
    EXPECT_EQ("1997-10-30", date);
}

TEST(DNSRegex, ExtractDate7)
{
    host_based_features_t f;
    auto date = f.get_dns_created(whois_google_com_br);
    EXPECT_EQ("1999-05-18", date);
}

TEST(DNSRegex, ExtractDate8)
{
    host_based_features_t f;
    auto date = f.get_dns_created(whois_doesnt_exists);
    EXPECT_EQ("", date);
}

TEST(DNSRegex, ExtractDateUpadted9)
{
    host_based_features_t f;
    auto date = f.get_dns_updated(whois_google_com);
    EXPECT_EQ("2019-09-09", date);
}

TEST(DNSRegex, ExtractDateUpadted10)
{
    host_based_features_t f;
    auto date = f.get_dns_updated(whois_google_com_br);
    EXPECT_EQ("2019-04-17", date);
}

TEST(DNSRegex, ExtractDateUpadted11)
{
    host_based_features_t f;
    auto date = f.get_dns_updated(whois_nic_cz);
    EXPECT_EQ("2016-11-22", date);
}

TEST(DNSRegex, ExtractDateUpadted12)
{
    host_based_features_t f;
    auto date = f.get_dns_updated(whois_nic_sk);
    EXPECT_EQ("2019-10-14", date);
}

TEST(DNSCreated, LongTimeAgo)
{
    host_based_features_t f;
    EXPECT_GT(0.1, /* > */ f.compute_value_dns_created(whois_nic_cz));
}

TEST(DNSCreated, FreshSite)
{
    host_based_features_t f;
    EXPECT_LT(0.9, /* < */ f.compute_value_dns_created(whois_staruch_sk));
}

TEST(DNSUpdated, ChangedRecently)
{
    host_based_features_t f;
    EXPECT_LT(0.9, /* < */ f.compute_value_dns_updated(whois_google_com_br));
}

TEST(DNSUpdated, Missing)
{
    host_based_features_t f;
    EXPECT_EQ(1, f.compute_value_dns_updated(whois_google_cn));
}

TEST(SSL, SubjectMatchHostname)
{
    host_based_features_t f("https://www.google.com");
    EXPECT_EQ(0, f.compute_value_ssl_subject(ssl_www_google_com));
}

TEST(SSL, SubjectDoesntMatchHostname)
{
    // google.com
    host_based_features_t f("https://google.com");
    EXPECT_EQ(1, f.compute_value_ssl_subject(ssl_google_com));
}

TEST(SSL, NoCert)
{
    host_based_features_t f("http://google.com");
    EXPECT_EQ(1, f.compute_value_ssl_subject(ssl_empty));
}

TEST(Headers, HSTS_false)
{
    host_based_features_t f;
    EXPECT_EQ(1, f.compute_value_hsts(curl_http_google));
}

TEST(Headers, HSTS_OK)
{
    host_based_features_t f;
    EXPECT_EQ(0, f.compute_value_hsts(curl_csp));
}

TEST(Headers, XSS_OK)
{
    host_based_features_t f;
    EXPECT_EQ(0, f.compute_value_xss_protection(curl_github_com));
}

TEST(Headers, XSS_MISSING)
{
    host_based_features_t f;
    EXPECT_EQ(1, f.compute_value_xss_protection(curl_https_google));
}

TEST(Headers, CSP_OK)
{
    host_based_features_t f;
    EXPECT_EQ(0, f.compute_value_csp(curl_csp));
}

TEST(Headers, CSP_MISSING)
{
    host_based_features_t f;
    EXPECT_EQ(1, f.compute_value_csp(curl_http_google));
}

TEST(Headers, X_FRAME_OK_DENY)
{
    host_based_features_t f;
    EXPECT_EQ(0, f.compute_value_x_frame(curl_github_com));
}

TEST(Headers, X_FRAME_OK_SAMEORIGIN)
{
    host_based_features_t f;
    EXPECT_EQ(0, f.compute_value_x_frame(curl_https_google));
}

TEST(Headers, X_FRAME_MISSING)
{
    host_based_features_t f;
    EXPECT_EQ(1, f.compute_value_x_frame(curl_paypal_com));
}

TEST(Headers, X_CONTENT_TYPE_OPTS_OK)
{
    host_based_features_t f;
    EXPECT_EQ(0, f.compute_value_x_content_type(curl_developer_mozzila_org));
}

TEST(Headers, X_CONTENT_TYPE_OPTS_MISSING)
{
    host_based_features_t f;
    EXPECT_EQ(1, f.compute_value_x_content_type(curl_paypal_com));
}

TEST(DomainSimilarity, SimilarDomain)
{
    host_based_features_t f("http://some.weird.domain.paypel.com");
    EXPECT_EQ(1, f.compute_similar_domain(similar_domain_paypel, sld_som_weird_domain_paypel_com));
}

TEST(DomainSimilarity, SameDomain)
{
    host_based_features_t f("http://paypal.com");
    EXPECT_EQ(0, f.compute_similar_domain(similar_domain_paypal, sld_paypal_com));
}


TEST(DomainSimilarity, Unknowndomain)
{
    host_based_features_t f("http://aiohsgoiashgoiashgoiahgoihaigo.com");
    EXPECT_EQ(1, f.compute_similar_domain(similar_domain_empty, sld_aiohsgoiashgoiashgoiahgoihaigo));
}

TEST(DomainSimilarity, MultipleCharDifferent)
{
    host_based_features_t f("http://gaagle.com");
    EXPECT_EQ(0.5, f.compute_similar_domain(similar_domain_gaagle, sld_gaagle));
}

TEST(SSLDate, Created1)
{
    host_based_features_t f;
    EXPECT_EQ("Oct 16 12:28:32 2019 GMT", f.get_ssl_created(ssl_www_google_com));
}

TEST(SSLDate, CreatedEmpty)
{
    host_based_features_t f;
    EXPECT_EQ("", f.get_ssl_created(ssl_empty));
}

TEST(SSLDate, Expiry)
{
    host_based_features_t f;
    EXPECT_EQ("Jan  8 12:28:32 2020 GMT", f.get_ssl_expire(ssl_www_google_com));
}

TEST(SSLDate, ExpireEmpty)
{
    host_based_features_t f;
    EXPECT_EQ("", f.get_ssl_created(ssl_empty));
}

TEST(SSLCreated, Now)
{
    host_based_features_t f;
    EXPECT_NEAR(0.5, f.compute_value_ssl_created(ssl_www_google_com), 0.1);
}

TEST(SSLCreated, NoValue)
{
    host_based_features_t f;
    EXPECT_EQ(1, f.compute_value_ssl_created(ssl_empty));
}

TEST(SSLExpire, Till2Years)
{
    host_based_features_t f;
    EXPECT_NEAR(0.5, f.compute_value_ssl_expire(ssl_www_google_com), 0.1);
}

TEST(SSLExpire, NoValue)
{
    host_based_features_t f;
    EXPECT_EQ(1, f.compute_value_ssl_expire(ssl_empty));
}
