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
