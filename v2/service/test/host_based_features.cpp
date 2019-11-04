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
