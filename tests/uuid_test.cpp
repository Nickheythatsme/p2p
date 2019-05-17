#include "../src/uuid.hpp"
#include "gtest/gtest.h"
#include <iostream>

#define PAUSE(T) std::this_thread::sleep_for(std::chrono::milliseconds(T)) // give logs time to output
#define PORT "8000"

using namespace p2p;
using std::cout;
using std::endl;

TEST(UUID, smoke)
{
    uuid u;
    cout << u << endl;
}