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

TEST(Str2Vec, SimpleString)
{
    std::string str = "Test";
    std::vector<std::string> vec{"Test"};
    EXPECT_EQ(vec, help_functions::str2vec(str));
}

TEST(Str2Vec, HTTPHeader)
{
    // equivavelnt to:
    // curl -s -I google.com | cat -A | head -n 2
    // HTTP/1.1 301 Moved Permanently^M$
    // Location: http://www.google.com/^M$
    auto str = "HTTP/1.1 301 Moved Permanently\r\nLocation: http://www.google.com/\r\n";
    std::vector<std::string> vec{"HTTP/1.1 301 Moved Permanently", "Location: http://www.google.com/"};
    EXPECT_EQ(vec, help_functions::str2vec(str));
}
