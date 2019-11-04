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
