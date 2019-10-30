#include "gtest/gtest.h"
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
        Key key;
        cout << key.to_string() << endl;
    }
}

void assure_randomness()
{
    size_t iterations = 1000;
    std::seed_seq seed {1};
    std::mt19937_64 gen (seed);
    std::vector<Key> keys {iterations};
    for (int i=0; i<iterations; ++i) {
        keys.emplace_back(Key(gen));
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
