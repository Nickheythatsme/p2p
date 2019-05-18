#include "../src/uuid.h"
#include "gtest/gtest.h"
#include <iostream>
#include <thread>

#define PAUSE(T) std::this_thread::sleep_for(std::chrono::milliseconds(T)) // give logs time to output

using namespace p2p;
using std::cout;
using std::endl;

TEST(UUID, smoke)
{
    auto u = UUID::init_random();
    cout << u << endl;
}

TEST(UUID, parse)
{
    std::string test_uuid = "4c0272e2-5901-43b9-be4b-73b219abaf2f";
    auto u = UUID::parse(test_uuid.c_str());
    auto u_string = u.to_string();
    cout << "test uuid:   " << test_uuid << endl
         << "parsed uuid: " << u << endl;
    PAUSE(100);
    ASSERT_TRUE(u_string == "4c0272e2-5901-43b9-be4b-73b219abaf2f");
}

TEST(UUID, parseShouldThrowException)
{
    std::string bad_format = "4c0272e205-901-43b9-be4b-73b219abaf2f";
    std::string invalid_uuid = "4c0272e2-5901-53b9-be4b-73b219abaf2f";
    try {
        auto u = UUID::parse(bad_format.c_str());
        FAIL() << "exception was not thrown for bad format uuid";
    }catch(std::invalid_argument& e) {
        std::cerr << e.what() << std::endl;
    }
    try {
        auto u = UUID::parse(invalid_uuid.c_str());
        FAIL() << "exception was not thrown for invalid uuid";
    }catch(std::invalid_argument& e) {
        std::cerr << e.what() << std::endl;
    }
}

