#include "gtest/gtest.h"
#include "features/all_features.h"

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
