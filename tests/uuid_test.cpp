#include "../src/util/uuid.h"
#include "gtest/gtest.h"
#include <iostream>
#include <vector>
#include <chrono>

using namespace p2p::util;
using std::cout;
using std::endl;

TEST(UUID, smoke)
{
    std::seed_seq seed{1};
    std::mt19937_64 gen(seed);
    auto u = UUID::init_random(gen);
    auto u_str = u.to_string();
    EXPECT_EQ("c3d20003-19c3-48aa-8493-89fcfa2b0b33", u_str);

    ASSERT_TRUE((u_str[19] & 0x8) == 0x8);
    ASSERT_TRUE(u_str[14] == '4');
}

TEST(UUID, assureRandomness)
{
    std::vector<UUID> uuids;
    std::seed_seq seed{1};
    std::mt19937_64 gen(seed);
    for(int i = 0; i < 10000; ++i) {
        auto new_uuid = UUID::init_random(gen);
        for(const auto &uuid : uuids) {
            auto u_str = uuid.to_string();
            ASSERT_TRUE((u_str[19] & 0x8) == 0x8);
            ASSERT_TRUE(u_str[14] == '4');
            if(uuid == new_uuid) {
                FAIL() << "a uuid was repeated \"" << uuid << "\" and \"" << new_uuid << "\"";
            }
            uuids.emplace_back(new_uuid);
        }
    }
}

TEST(UUID, parse)
{
    std::string test_uuid = "4c0272e2-5901-43b9-be4b-73b219abaf2f";
    auto u = UUID::parse(test_uuid.c_str());
    auto u_string = u.to_string();
    cout << "test uuid:   " << test_uuid << endl
         << "parsed uuid: " << u << endl;
    ASSERT_EQ(u_string, "4c0272e2-5901-43b9-be4b-73b219abaf2f");
}

TEST(UUID, speedParse)
{
    std::string test_uuid = "4c0272e2-5901-43b9-be4b-73b219abaf2f";
    auto start = std::chrono::steady_clock::now();
    for (int i=0; i<10000; ++i)
    {
        auto u = UUID::parse(test_uuid.c_str());
    }
    auto end = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    cout << "Completed 10,000 uuid_parse in " << elapsed.count() << " us" << endl;
}

TEST(UUID, parseShouldThrowException)
{
    std::string bad_format = "4c0272e205-901-43b9-be4b-73b219abaf2f";
    std::string invalid_uuid = "4c0272e2-5901-53b9-be4b-73b219abaf2f";
    ASSERT_THROW(
    auto u = UUID::parse(bad_format.c_str()),
    std::invalid_argument
    );
    ASSERT_THROW(
    auto u = UUID::parse(invalid_uuid.c_str()),
    std::invalid_argument
    );
}
