#include "gtest/gtest.h"
#include "features/all_features.h"

TEST(Utility, GetOutPutFromProgram)
{
    feature::html_features f;
    std::string cmd = R"(echo "1\n2\n34")";
    auto lines = f.get_output_from_program(cmd.c_str());
    std::vector<std::string> expected = {
        "1\n",
        "2\n",
        "34\n"
    };
    EXPECT_EQ(lines, expected);
}

TEST(InputTag, HasInputTags)
{
    feature::html_features f;
    int count_of_input_tags = 10;
    EXPECT_EQ(1, f.compute_value_input_tag(count_of_input_tags));
}

TEST(InputTag, NoInputTags)
{
    feature::html_features f;
    int count_of_input_tags = 0;
    EXPECT_EQ(0, f.compute_value_input_tag(count_of_input_tags));
}
