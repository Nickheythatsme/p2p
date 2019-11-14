
#ifndef P2P_KEY
#define P2P_KEY

#include <memory>
#include <cstdint>
#include <random>
#include <string>
#include "../networking/serialize.h"

namespace p2p {
namespace util {

class Key: public networking::Serializable
{
    public:
        Key() = default;
        ~Key() = default;
        Key(const Key& rhs);
        Key(Key &&rhs) noexcept;
        Key(std::unique_ptr<uint64_t[]> &&data);
        static Key init_random();
        static Key init_random(std::mt19937_64& generator);

        friend std::istream &operator>>(std::istream &in, Key &u);
        friend std::ostream &operator<<(std::ostream &out, const Key &u);
        std::istream& unserialize(std::istream& in);
        std::ostream& serialize(std::ostream& out) const;
        std::string to_string() const;

        static const size_t KEY_NBYTES = 4;
    private:
        std::unique_ptr<uint64_t[]> data {new uint64_t[KEY_NBYTES]};
};

} // namespace util
} // namespace p2p
#endif // P2P_KEY

