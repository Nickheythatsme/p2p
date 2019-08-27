#include "../src/util/uuid.h"
#include "gtest/gtest.h"
#include <iostream>
#include <vector>

using namespace p2p::util;
using std::cout;
using std::endl;

TEST(UUID, smoke)
{
    std::seed_seq seed{1};
    std::mt19937_64 gen(seed);
    auto u = UUID::init_random(gen);
    cout << u << endl;
    ASSERT_EQ("330b2bfa-fc89-4304-aa68-c3190300d2c3", u.to_string());
}

TEST(UUID, assureRandomness)
{
    std::vector<UUID> uuids{};
    std::seed_seq seed{1};
    std::mt19937_64 gen(seed);
    for(int i = 0; i < 10000; ++i) {
        auto new_uuid = UUID::init_random(gen);
        for(const auto &uuid : uuids) {
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
    ASSERT_TRUE(u_string == "4c0272e2-5901-43b9-be4b-73b219abaf2f");
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
