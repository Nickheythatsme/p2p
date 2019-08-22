//
// Created by Nick_Grout on 2019-08-20.
//

#include <iostream>
#include <chrono>
#include <vector>
#include <cstdlib>
#include <string>
#include <map>
#include "gtest/gtest.h"
#include "../src/hash_table.h"
#include "../src/hash.hpp"

using std::cout;
using std::cerr;
using std::endl;

using namespace p2p;

template<class T>
std::vector<T> generate_random_numbers(size_t len, T limit) {
  srand(1);
  std::vector<T> values;
  for (int i = 0; i < len; ++i) {
    values.emplace_back(rand() % limit);
  }
  return values;
}

template<class K, class V>
class DebugableHashTable : public HashTable<K, V> {
  public:
    DebugableHashTable() = default;
    DebugableHashTable(size_t len) : HashTable<K, V>(len) {}
    void print_table() const {
      for (int i = 0; i < HashTable<K, V>::len; ++i) {
        auto current = &HashTable<K, V>::table[i];
        while (*current) {
          std::cout << (*current)->key << ":" << (*current)->value << " -> ";
          current = &(*current)->next;
        }
        std::cout << "NULL" << std::endl;
      }
    }

};

TEST(HashTable, smoke) {
  HashTable<int, int> hashTable(100);
  HashTable<int, int> hashTableDefaultConstructor;
}

TEST(HashTable, putTest) {
  DebugableHashTable<int, int> hashTable(10);
  hashTable.put(1, 1);
  hashTable.put(1, 2);
  hashTable.put(1, 3);
  hashTable.print_table();
}

void test_const_get(const HashTable<int, int> &a, int value) {
  const auto val = a.get(value);
}

TEST(HashTable, getTest) {
  DebugableHashTable<int, int> hashTable(10);
  auto values = generate_random_numbers<int>(10, 2048);
  for (auto &value : values) {
    hashTable.put(value, value);
  }

  for (auto &value : values) {
    EXPECT_TRUE(hashTable.contains(value));
  }
  EXPECT_FALSE(hashTable.contains(2048));
  EXPECT_THROW(hashTable.get(2048), hash_table_exception);

  test_const_get(hashTable, values[0]);
}

TEST(HashTable, speed) {
  auto values = generate_random_numbers<int>(2048, 2048);
  size_t hashTableMicroseconds{0};
  size_t stdMapMicroseconds{0};

  // Time HashTable
  {
    HashTable<int, int> hashTable;
    auto start = std::chrono::steady_clock::now();
    for (auto &value : values) {
      hashTable.put(value, value);
    }
    for (auto &value : values) {
      hashTable.get(value);
    }
    auto end = std::chrono::steady_clock::now();
    hashTableMicroseconds = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    cout << "Completed HashTable in " << hashTableMicroseconds << " us" << endl;
  }

  // Time std::map
  {
    std::map<int, int> map;
    auto start = std::chrono::steady_clock::now();
    for (auto &value : values) {
      map[value] = value;
    }
    for (auto &value : values) {
      map.at(value);
    }
    auto end = std::chrono::steady_clock::now();
    stdMapMicroseconds = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    cout << "Completed std::map in " << stdMapMicroseconds << " us" << endl;
  }
  ASSERT_LT(hashTableMicroseconds, stdMapMicroseconds);
}

TEST(HashTable, usingHashObject) {
  HashTable<HashObject, std::string> hashTable;
  HashBuilder builder;

  for (int i = 0; i < 10; ++i) {
    builder.write((unsigned char *) &i, sizeof(int));
    auto hashObject = builder.finalize();
    builder.reset();
    hashTable.put(hashObject, std::to_string(i));
  }

  int value = 0;
  builder.write((unsigned char *) &value, sizeof(int));
  auto hashObject = builder.finalize();
  builder.reset();
  ASSERT_NO_THROW(hashTable.get(hashObject));
}
