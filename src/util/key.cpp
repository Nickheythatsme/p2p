#include "key.h"
#include <chrono>
#include <sstream>

namespace p2p {
namespace util {

Key::Key()
{
    auto now = std::chrono::steady_clock::now();
    auto since_epoch = now.time_since_epoch();
    auto now_cast = std::chrono::duration_cast<std::chrono::microseconds>(since_epoch).count();
    std::seed_seq seed {now_cast};
    std::mt19937_64 gen (seed);
    init(gen);
}

Key::Key(std::mt19937_64& generator)
{
    init(generator);
}

void Key::init(std::mt19937_64& gen)
{
    data1 = gen();
    data2 = gen();
    for (int i=0; i<1000; ++i) {
        data1 <<= 8;
        data1 ^= gen();
        data2 <<= 8;
        data2 ^= gen();
    }
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

} // namespace util
} // namespace p2p
