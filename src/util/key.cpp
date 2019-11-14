#include "key.h"
#include <cstring>
#include <sstream>


namespace p2p {
using namespace networking;
namespace util {

Key::Key(const Key& rhs)
{
    std::memcpy(data.get(), rhs.data.get(), KEY_NBYTES * sizeof(uint64_t));
}

Key::Key(Key &&rhs) noexcept:
    data(std::move(rhs.data))
{
}

Key::Key(std::unique_ptr<uint64_t[]> &&data):
    data(std::move(data))
{
}

Key Key::init_random()
{
    auto now = std::chrono::steady_clock::now();
    auto since_epoch = now.time_since_epoch();
    auto now_cast = std::chrono::duration_cast<std::chrono::microseconds>(since_epoch).count();
    std::seed_seq seed {now_cast};
    std::mt19937_64 gen (seed);
    return Key::init_random(gen);
}

Key Key::init_random(std::mt19937_64& gen)
{
    auto data = std::make_unique<uint64_t[]>(KEY_NBYTES);
    for (int i=0; i< KEY_NBYTES; ++i) {
        data[i] = gen();
    }
    return std::move(data);
}

std::istream &operator>>(std::istream &in, Key &u)
{
    return u.unserialize(in);
}

std::ostream &operator<<(std::ostream &out, const Key &u)
{
    return u.serialize(out);
}

std::ostream& Key::serialize(std::ostream& out) const
{
    writeNetworkLongLong(out, data[0]);
    writeNetworkLongLong(out, data[1]);
    writeNetworkLongLong(out, data[2]);
    return writeNetworkLongLong(out, data[3]);
}

std::istream& Key::unserialize(std::istream& in)
{
    data[0] = readNetworkLong(in);
    data[1] = readNetworkLong(in);
    data[2] = readNetworkLong(in);
    data[3] = readNetworkLong(in);
    return in;
}

std::string Key::to_string() const
{
    std::stringstream ss;
    ss << std::hex << data[0] << data[1] << data[2] << data[3];
    return ss.str();
}

} // namespace util
} // namespace p2p
