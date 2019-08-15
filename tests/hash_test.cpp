//
// Created by Nick Grout on 2019-08-13.
//

#define _DEBUG
#include "gtest/gtest.h"
#include <iostream>
#include "../src/hash.hpp"
#include <fstream>
#include <thread>
#include <chrono>

using std::cout;
using std::cerr;
using std::endl;
using std::ifstream;

using namespace p2p;

TEST(SmokeTests, ExampleTest) {
    cout << "starting test" << endl;
    Hash256 hash256;
    ifstream fin("../CMakeLists.txt.in");
    if (!fin) {
        cerr << "Could not open file " << "../CMakeLists.txt.in" << endl;
    }
    // hash256.write("testing!", strlen("testing!"));
    hash256.write(fin);
    auto hash = hash256.finalize();
    cout << hash << endl;
    ASSERT_EQ(hash, "e899fac9a6c4ff729837d8c78ce61d220000");
}

