#include <iostream>
#include "gtest/gtest.h"

#include "features/ip_address.h"

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
    EXPECT_EQ(0., f.compute_value());
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
    EXPECT_EQ(0., f.compute_value());
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
