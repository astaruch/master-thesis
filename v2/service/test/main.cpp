#include <iostream>
#include "gtest/gtest.h"

#include "features/all_features.h"


TEST(HostIPAddress, NoIPAddress)
{
    feature::ip_address f;
    f.set_url("http://google.com");
    EXPECT_EQ(0., f.compute_value());
}

TEST(HostIPAddress, ClassicDotForm)
{
    feature::ip_address f;
    f.set_url("http://138.197.136.185/bnc/National\%20Bank\%20Online.html");
    EXPECT_EQ(1., f.compute_value());
}

TEST(HostIPAddress, OctalForm)
{
    feature::ip_address f;
    f.set_url("https://0254.0331.027.0356");
    EXPECT_EQ(1., f.compute_value());
}

TEST(HostIPAddress, DecimalForm)
{
    feature::ip_address f;
    f.set_url("https://2899908590");
    EXPECT_EQ(1., f.compute_value());
}

TEST(HostIPAddress, HexadecimalForm)
{
    feature::ip_address f;
    f.set_url("http://0x308f647/-credit-agricole-france/");
    EXPECT_EQ(1., f.compute_value());
}

TEST(URLLength, ShortURL)
{
    feature::url_length f;
    f.set_url("https://google.com");
    EXPECT_GT(0.1, f.compute_value());
}

TEST(URLLength, LongURL)
{
    feature::url_length f;
    f.set_url("http://abielonline.com/wp-includes/alibaba/vqcr8bp0gud&amp;lc=1033&amp;id=64855&amp;mkt=en-us&amp;cbcxt=mai&amp;snsc.php?email=santmmafs@etttbt.com%20&amp;?email=rxeitsie@sbeilmep.com%20");
    EXPECT_EQ(1, f.compute_value());
}

TEST(HostLength, Short)
{
    feature::host_length f;
    f.set_url("http://google.com");
    EXPECT_GT(0.01, f.compute_value());
}


TEST(PathLength, NoPath)
{
    feature::path_length f;
    f.set_url("http://abielonline.com/");
    EXPECT_GT(0.01, f.compute_value());
}

TEST(QueryLength, NoQuery)
{
    feature::query_length f;
    f.set_url("http://abielonline.com");
    EXPECT_GT(0.01, f.compute_value());
}

TEST(FragmentLength, NoFragment)
{
    feature::fragment_length f;
    f.set_url("http://abielonline.com/");
    EXPECT_GT(0.01, f.compute_value());
}

TEST(UserInfo, HasUserInfo)
{
    feature::user_info f;
    f.set_url("http://postgres:password@www.pgadmin.com");
    EXPECT_EQ(1, f.compute_value());
}

TEST(UserInfo, PhishingUserInfo)
{
    feature::user_info f;
    f.set_url("http://paypal.com@phishing-site.com");
    EXPECT_EQ(1, f.compute_value());
}

TEST(UserInfo, NoUserInfo)
{
    feature::user_info f;
    f.set_url("http://google.com");
    EXPECT_EQ(0, f.compute_value());
}

TEST(UserInfo, RealURL)
{
    feature::user_info f;
    f.set_url("ftp://cust-r2:Alpc2p3O@update.symantec.com/opt/content/onramp/minitri.flg");
    EXPECT_EQ(1, f.compute_value());
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
