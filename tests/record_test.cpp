//
// Created by Nick Grout on 2019-08-26.
//


#include "gtest/gtest.h"
#include "record.h"
#include "util/hash.hpp"
#include <random>
#include <chrono>
#include <vector>
#include <iostream>
#include <sstream>

using std::cout;
using std::endl;
using namespace p2p;

Record generate_record()
{
    auto now = std::chrono::steady_clock::now();
    auto since_epoch = now.time_since_epoch();
    auto now_cast = std::chrono::duration_cast<std::chrono::milliseconds>(since_epoch).count();
    std::seed_seq seed {now_cast};
    std::mt19937_64 gen (seed);
    int len = gen() % 50;

    std::string contents;
    for (int i=0; i<len; ++i)
    {
        char current_char = (gen() % 94) + 32;
        contents += current_char;
    }
    return Record::build(contents);
}

void print_record(const Record &r)
{
    cout << "UUID: " << r.get_uuid() << endl
         << "HASH: " << r.get_hash256().to_string() << endl
         << "Record contents: " << r.get_record_contents() << endl
         << endl;
}

TEST(Record, smoke) {
    auto record = generate_record();
}

TEST(Record, serialize_unserialize)
{
    auto record = generate_record();
    std::stringstream ss;
    record.serialize(ss);


    Record record2;
    record2.unserialize(ss);
    print_record(record);
    print_record(record2);

    ASSERT_EQ(record.get_uuid(), record2.get_uuid());
    ASSERT_EQ(record.get_hash256(), record2.get_hash256());
    ASSERT_EQ(record.get_record_contents(), record2.get_record_contents());
}

TEST(Record, unserialize_error)
{
    // Create the record
    HashBuilder builder;
    builder.write(reinterpret_cast<const unsigned char*>("test"), 4);
    Hash256 hash256 = builder.finalize();
    Record record(util::UUID::init_random(), hash256, "not the right content");

    // Serialize it
    std::stringstream ss;
    record.serialize(ss);


    // Unserialize it, expect error
    Record record_unserialize;
    ASSERT_THROW(record_unserialize.unserialize(ss), networking::serialize_exception);
}

