#include "gtest/gtest.h"
#include "help_functions.h"

TEST(Utility, GetOutPutFromProgram)
{
    std::string cmd = R"(echo "1\n2\n34")";
    auto lines = help_functions::get_output_from_program(cmd.c_str());
    std::vector<std::string> expected = {
        "1",
        "2",
        "34"
    };
    EXPECT_EQ(lines, expected);
}

TEST(Utility, GetOutPutFromProgramEmptyLine)
{
    std::string cmd = R"(echo "")";
    auto lines = help_functions::get_output_from_program(cmd.c_str());
    std::vector<std::string> expected = {
        ""
    };
    EXPECT_EQ(lines, expected);
}

TEST(Utility, CheckThatEndLinesFromHeadersAreRemoved)
{
    std::string cmd = R"(printf 'GET / HTTP/1.1\r\nHost: www.example.com\r\nConnection: close\r\n\r\n')";
    auto lines = help_functions::get_output_from_program(cmd.c_str());
    std::vector<std::string> expected = {
        "GET / HTTP/1.1",
        "Host: www.example.com",
        "Connection: close",
        ""
    };
    EXPECT_EQ(lines, expected);
}

TEST(HelpFunctionURL, GetSLD1)
{
    auto sld = help_functions::get_sld("http://some.weird.domain.paypel.com");
    EXPECT_EQ(sld, "paypel");
}

TEST(HelpFunctionURL, GetSLD2)
{
    auto sld = help_functions::get_sld("http://aiohsgoiashgoiashgoiahgoihaigo.com.br");
    EXPECT_EQ(sld, "aiohsgoiashgoiashgoiahgoihaigo");
}

TEST(HelpFunctionURL, GetSLD3)
{
    auto sld = help_functions::get_sld("http://google.cz");
    EXPECT_EQ(sld, "google");
}
