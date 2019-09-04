//
// Created by Nick Grout on 2019-08-26.
//


#include "gtest/gtest.h"
#include "record.h"
#include "util/hash.hpp"
#include "util/hash_table.h"
#include <vector>

using std::cout;
using std::endl;
using namespace p2p;

TEST(Record, smoke) {
    auto local = LocalRecord::CreateLocalRecord("Makefile");
    local.retrieve(cout);
}

TEST(Record, hashTable) {
    HashTable<UUID, std::unique_ptr<Record>> hash_table;
    auto local = LocalRecord::CreateLocalRecord("Makefile");
    auto uuid = local.get_uuid();
    hash_table.put(
        uuid,
        std::unique_ptr<Record> (new LocalRecord(std::move(local)))
    );

    auto local_retrieved = std::move(hash_table.get(uuid));
    local_retrieved->retrieve(cout);
}
