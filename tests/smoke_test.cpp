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
    hash256.update("testing", strlen("testing"));
    const char* payload;
    cout << hash256.final() << endl;
}

TEST(SmokeTests, FileTest) {
    File file;
}

