//
// Created by Nick Grout on 2019-08-13.
//

#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include "gtest/gtest.h"
#include "../src/hash.hpp"
#include "../src/file.hpp"

using std::cout;
using std::cerr;
using std::endl;
using std::ifstream;

using namespace p2p;

TEST(SmokeTests, Hash) {
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
    ASSERT_EQ(hash, "e899fac9a6c4ff72d111cc8400008ce61d22");
}

TEST(SmokeTests, FileTest) {
    File file;
}

