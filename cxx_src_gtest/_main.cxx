#include <gtest/gtest.h>
#pragma

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    testing::FLAGS_gtest_filter = "*Example01*";
    testing::FLAGS_gtest_filter = "*Example02*";
    testing::FLAGS_gtest_filter = "*Example*";
    return RUN_ALL_TESTS();
}
