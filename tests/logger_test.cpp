// 
// Nicholas Grout 4/30
//
#include "gtest/gtest.h"
#include "../src/util/logger.hpp"

using namespace p2p;

TEST(Logger, Smoke) {
    const char* address_test = "address_test";
    auto cout_buf = std::cout.rdbuf();
    auto ostream_ptr = new std::ostream(cout_buf);


    /*
    *log << "test!" << "test2";
    *log << &address_test;
    */
}

