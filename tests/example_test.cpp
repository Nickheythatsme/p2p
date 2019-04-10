#include "gtest/gtest.h"
#include <iostream>

using std::cout;
using std::endl;

TEST(SmokeTests, TestingWorks) {
    EXPECT_EQ(1,1);
    cout << "testing logging..." << endl;
}


