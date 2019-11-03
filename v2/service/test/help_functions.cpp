#include "gtest/gtest.h"
#include "help_functions.h"

TEST(Utility, GetOutPutFromProgram)
{
    std::string cmd = R"(echo "1\n2\n34")";
    auto lines = help_functions::get_output_from_program(cmd.c_str());
    std::vector<std::string> expected = {
        "1\n",
        "2\n",
        "34\n"
    };
    EXPECT_EQ(lines, expected);
}
