#include <gtest/gtest.h>
#include <filesystem>

#include "libs/config.h"

namespace fs = std::filesystem;

namespace phishapp {

struct ConfigTest : public ::testing::Test {
    const char* path = "config.yaml";
    std::error_code ec;

    void
    SetUp() override
    {
        phishapp::Config::set_path("config.yaml");
    }

    void
    TearDown() override
    {
        fs::remove(path, ec);
        Config::destroy();
    }
};

TEST_F(ConfigTest, default_weight_length)
{
    EXPECT_EQ(0, phishapp::Config::weight_length());
}

TEST_F(ConfigTest, default_weight_depth)
{
    EXPECT_EQ(0, Config::weight_depth());
}

TEST_F(ConfigTest, default_weight_special_chars)
{
    EXPECT_EQ(0, Config::weight_special_chars());
}

TEST_F(ConfigTest, default_weight_special_keywords)
{
    EXPECT_EQ(0, Config::weight_special_keywords());
}

TEST_F(ConfigTest, default_weight_hostname_ip)
{
    EXPECT_EQ(0, Config::weight_hostname_ip());
}

TEST_F(ConfigTest, default_weight_non_std_port)
{
    EXPECT_EQ(0, Config::weight_non_std_port());
}

TEST_F(ConfigTest, default_weight_non_std_tld)
{
    EXPECT_EQ(0, Config::weight_non_std_tld());
}

TEST_F(ConfigTest, default_weight_query_script)
{
    EXPECT_EQ(0, Config::weight_query_script());
}

TEST_F(ConfigTest, default_weight_query_html)
{
    EXPECT_EQ(0, Config::weight_query_html());
}

TEST_F(ConfigTest, default_weight_chars_freq)
{
    EXPECT_EQ(0, Config::weight_chars_freq());
}



} // namespace phishapp