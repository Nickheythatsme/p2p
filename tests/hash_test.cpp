//
// Created by Nick Grout on 2019-08-13.
//

#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include "gtest/gtest.h"
#include "../src/hash.hpp"

using std::cout;
using std::cerr;
using std::endl;
using std::ifstream;

using namespace p2p;

std::stringstream generate_stream(size_t len)
{
    std::string to_fill {"this is a test string"};
    std::stringstream ss;
    for (int i=0; i<len; ++i)
    {
        ss << to_fill;
    }
    return ss;
}

TEST(HashTest, SmokeTest) {
    HashBuilder hashBuilder;
    auto ss = generate_stream(100);
    ss >> hashBuilder;
    auto hashObject = hashBuilder.finalize();
    std::stringstream ss_out;
    ss_out << hashObject;
    ASSERT_EQ(ss_out.str(), "1fa8c9f82382d114db6f406570faaaf6b77aa28281357ebc2c4756e9cd15");
}

TEST(HashTest, Equality)
{
    {
        HashBuilder hashBuilder;
        auto ss_1 = generate_stream(100);
        auto ss_2 = generate_stream(101);
        ss_1 >> hashBuilder;
        auto hash_1 = hashBuilder.finalize();

        ss_2 >> hashBuilder.reset();
        auto hash_2 = hashBuilder.finalize();
        ASSERT_NE(hash_1, hash_2);
        ASSERT_LT(hash_1, hash_2);
    }
}

TEST(HashTest, TestBuilder)
{
    {
        HashBuilder hashBuilder;
        hashBuilder.write(reinterpret_cast<const unsigned char*>("this is a test"), strlen("this is a test"));
        hashBuilder.finalize();

        ASSERT_THROW(hashBuilder.finalize(), hash_exception);
    }

    {
        HashBuilder hashBuilder;
        hashBuilder.write(reinterpret_cast<const unsigned char*>("this is a test"), strlen("this is a test"));
        hashBuilder.finalize();

        ASSERT_THROW(
            hashBuilder.write(reinterpret_cast<const unsigned char*>("this is a test"), strlen("this is a test")),
            hash_exception
        );
    }
}
