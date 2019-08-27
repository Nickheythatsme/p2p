//
// Created by Nick Grout on 2019-08-13.
//

#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include "gtest/gtest.h"
#include "../src/util/hash.hpp"

using std::cout;
using std::cerr;
using std::endl;
using std::ifstream;

using namespace p2p::util;

std::stringstream generate_stream(size_t len) {
  char to_fill{' '};
  std::stringstream ss;
  for (int i = 0; i < len; ++i) {
    ss << to_fill++;
    to_fill %= '~';
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
  ASSERT_EQ(ss_out.str(), "29a3ad1d1367654a736740712e3312f34c10938a99a37eee7a2bb61912761c7f");
}

TEST(HashTest, EqualityOperators) {
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

TEST(HashTest, ModOperator) {
  HashBuilder hashBuilder;
  hashBuilder.write(reinterpret_cast<const unsigned char *>("this is a test"), strlen("this is a test"));
  auto hashObject = hashBuilder.finalize();
  ASSERT_EQ(hashObject % 100, 62);
}

TEST(HashTest, TestBuilder) {
  {
    HashBuilder hashBuilder;
    hashBuilder.write(reinterpret_cast<const unsigned char *>("this is a test"), strlen("this is a test"));
    hashBuilder.finalize();

    ASSERT_THROW(hashBuilder.finalize(), hash_exception);
  }

  {
    HashBuilder hashBuilder;
    hashBuilder.write(reinterpret_cast<const unsigned char *>("this is a test"), strlen("this is a test"));
    hashBuilder.finalize();

    ASSERT_THROW(
        hashBuilder.write(reinterpret_cast<const unsigned char *>("this is a test"), strlen("this is a test")),
        hash_exception
    );
  }
}

TEST(HashTest, LoadTest) {
  auto data_chunk = generate_stream(100000).str();
  auto start = std::chrono::steady_clock::now();
  HashBuilder builder;
  builder.write(
      (const unsigned char *) data_chunk.c_str(),
      data_chunk.size()
  );
  auto hash = builder.finalize();
  ASSERT_EQ(hash % 100, 49);
  auto end = std::chrono::steady_clock::now();
  auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
  cout << "Completed hash in " << elapsed.count() << " us" << endl;
}
