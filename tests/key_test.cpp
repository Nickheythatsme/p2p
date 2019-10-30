#include "gtest/gtest.h"
#include "networking/serialize.h"
#include "util/key.h"
#include <arpa/inet.h>
#include <sstream>
#include <iostream>
#include <vector>
#include <random>
#include <thread>


using std::cout;
using std::cerr;
using std::endl;
using std::stringstream;
using namespace p2p;
using namespace util;

TEST(Key, smoke) {
    for (int i=0; i<10; ++i) {
        Key key = Key::init_random();
        cout << key.to_string() << endl;
    }
}

void assure_randomness()
{
    size_t iterations = 100;
    std::seed_seq seed {1};
    std::mt19937_64 gen (seed);
    std::vector<Key> keys;
    for (int i=0; i<iterations; ++i) {
        keys.emplace_back(Key::init_random(gen));
    }
    for (int i=0; i<iterations; ++i) {
        Key target = keys[i];
        for (int k=0; k<iterations; ++k) {
            if (k != i && keys[k] == target) {
                FAIL() << "key was repeated: " 
                    << k << ") "
                    << target.to_string() 
                    << " " 
                    << keys[k].to_string() 
                    << "\n";
            }
        }
    }
}

TEST(Key, assureRandomness) {
    std::vector<std::thread> threads;
    for (int i=0; i<4; ++i) {
        threads.emplace_back(std::thread(assure_randomness));
    }
    for (std::thread &t: threads)
    {
        t.join();
    }
}

TEST(Key, serialize_unserialize) {
    {
        std::stringstream ss;
        Key key;
        networking::writeNetworkLongLong(ss, 0x1122334455667788);
        networking::writeNetworkLongLong(ss, 0x0123456789abcdef);
        key.unserialize(ss);
        ASSERT_EQ("1122334455667788123456789abcdef", key.to_string());
    }

    {
        std::stringstream ss;
        Key unserialize_key;
        auto key = Key::init_random();
        key.serialize(ss);
        unserialize_key.unserialize(ss);

        cout << key.to_string() << endl;
        cout << unserialize_key.to_string() << endl;
        ASSERT_EQ(key, unserialize_key);
    }

}
