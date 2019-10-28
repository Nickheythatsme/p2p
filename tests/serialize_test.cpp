
#include "gtest/gtest.h"
#include "networking/serialize.h"
#include <arpa/inet.h>
#include <sstream>
#include <iostream>
#include <vector>


using std::cout;
using std::endl;
using std::stringstream;
using namespace p2p;
using namespace p2p::networking;

TEST(Serialize, readLongLong) {
    stringstream ss;
    uint64_t value = 0x0123456789abcdef;
    ss.write((char*)&value, sizeof(value));
    uint64_t response = readNetworkLongLong(ss);
    printf("expected: efcdab8967452301\n");
    printf("observed: %lx\n", response);
    ASSERT_EQ(0xefcdab8967452301, response);
}

TEST(Serialize, readLong) {
    stringstream ss;
    uint32_t value = 0x01234567;
    uint32_t network_value = htonl(value);
    ss.write((char*)&network_value, sizeof(network_value));
    uint32_t response = readNetworkLong(ss);
    ASSERT_EQ(value, response);
}

TEST(Serialize, readShort) {
    stringstream ss;
    uint16_t value = 0x0123;
    uint16_t network_value = htons(value);
    ss.write((char*)&network_value, sizeof(network_value));
    uint16_t response = readNetworkShort(ss);
    ASSERT_EQ(value, response);
}

TEST(Serialize, writeLongLong) {
    stringstream ss;
    uint64_t value = 0x0123456789abcdef;
    uint64_t expected = 0xefcdab8967452301;
    writeNetworkLongLong(ss, value);

    uint64_t observed = 0;
    ss.read((char*)&observed, sizeof(observed));
    printf("expected: %lx\n", expected);
    printf("observed: %lx\n", observed);
    ASSERT_EQ(expected, observed);
}

TEST(Serialize, writeLong) {
    stringstream ss;
    uint32_t value = 0x01234567;
    uint32_t expected = htonl(value);
    writeNetworkLong(ss, value);

    uint32_t observed = 0;
    ss.read((char*)&observed, sizeof(observed));
    ASSERT_EQ(expected, observed);
}

TEST(Serialize, writeShort) {
    stringstream ss;
    uint16_t value = 0x0123;
    uint16_t expected = htons(value);
    writeNetworkShort(ss, value);

    uint16_t observed = 0;
    ss.read((char*)&observed, sizeof(observed));
    ASSERT_EQ(expected, observed);
}

