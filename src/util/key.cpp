#include "key.h"
#include <chrono>
#include <sstream>

namespace p2p {
namespace util {

Key Key::init_random()
{
    auto now = std::chrono::steady_clock::now();
    auto since_epoch = now.time_since_epoch();
    auto now_cast = std::chrono::duration_cast<std::chrono::microseconds>(since_epoch).count();
    std::seed_seq seed {now_cast};
    std::mt19937_64 gen (seed);
    return Key::init_random(gen);
}

Key Key::init_random(std::mt19937_64& generator)
{
    uint64_t data1 = generator();
    uint64_t data2 = generator();
    for (int i=0; i<1000; ++i) {
        data1 <<= 8;
        data1 ^= generator();
        data2 <<= 8;
        data2 ^= generator();
    }
    return Key(data1, data2);
}

Key::Key(uint64_t data1, uint64_t data2):
    data1(data1),
    data2(data2)
{
}

Key::Key():
    data1(0),
    data2(0)
{
}

Key& Key::operator=(const Key& rhs)
{
    data1 = rhs.data1;
    data2 = rhs.data2;
    return *this;
}

std::string Key::to_string() const
{
    std::stringstream ss;
    ss << std::hex << data1 << std::hex << data2;
    return ss.str();
}

int Key::compare(const Key &rhs) const
{
    return (rhs.data1 - data1) + (rhs.data2 - data2);
}

bool operator<(const Key& lhs, const Key& rhs)
{
    return lhs.compare(rhs) < 0;
}

bool operator<=(const Key& lhs, const Key& rhs)
{
    return lhs.compare(rhs) <= 0;
}

bool operator==(const Key& lhs, const Key& rhs)
{
    return lhs.compare(rhs) == 0;
}

bool operator!=(const Key& lhs, const Key& rhs)
{
    return lhs.compare(rhs) != 0;
}

bool operator>(const Key& lhs, const Key& rhs)
{
    return lhs.compare(rhs) > 0;
}

bool operator>=(const Key& lhs, const Key& rhs)
{
    return lhs.compare(rhs) >= 0;
}

std::ostream& Key::serialize(std::ostream& out) const
{
    networking::writeNetworkLongLong(out, data1);
    networking::writeNetworkLongLong(out, data2);
    return out;
}

std::istream& Key::unserialize(std::istream& in)
{
    data1 = networking::readNetworkLongLong(in);
    data2 = networking::readNetworkLongLong(in);
    return in;
}

} // namespace util
} // namespace p2p
