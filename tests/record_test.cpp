//
// Created by Nick Grout on 2019-08-26.
//


#include "gtest/gtest.h"
#include "record.h"
#include "util/hash.hpp"
#include <vector>

using std::cout;
using std::endl;
using namespace p2p;

TEST(Record, smoke) {
    auto local = LocalRecord::CreateLocalRecord("Makefile");
    local.retrieve(cout);
}
