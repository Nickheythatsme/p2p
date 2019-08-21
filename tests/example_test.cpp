#include "gtest/gtest.h"
#include <iostream>

using std::cout;
using std::endl;

TEST(SmokeTests, ExampleTest) {
  EXPECT_EQ(1, 1);
  cout << "example test..." << endl;
}


