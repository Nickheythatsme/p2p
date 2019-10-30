#include <string>
#include <memory>
#include <cstdint>
#include <fstream>
#include <random>
#include "../networking/serialize.h"

#ifndef P2P_KEY
#define P2P_KEY

namespace p2p {
namespace util {

class Key: public networking::Serializable
{
public:
    static Key init_random();
    static Key init_random(std::mt19937_64& generator);
    Key();
    Key(const Key &rhs) = default;
    Key(Key &&rhs) = default;
    ~Key() = default;
    // Assignment operator
    Key& operator=(const Key& rhs);
    std::string to_string() const;
    // Relational & Equality operators and function
    int compare(const Key &rhs) const;
    friend bool operator<(const Key& lhs, const Key& rhs);
    friend bool operator<=(const Key& lhs, const Key& rhs);
    friend bool operator==(const Key& lhs, const Key& rhs);
    friend bool operator!=(const Key& lhs, const Key& rhs);
    friend bool operator>(const Key& lhs, const Key& rhs);
    friend bool operator>=(const Key& lhs, const Key& rhs);

    // Serialize functions
    std::ostream& serialize(std::ostream& out) const;
    std::istream& unserialize(std::istream& in);
    friend std::ostream& operator<<(std::ostream& out, const Key &rhs);
    friend std::istream& operator>>(std::istream& in, Key &rhs);
    static const size_t NLONG = 2; // two ints
private:
    Key(uint64_t data1, uint64_t data2);
    uint64_t data1;
    uint64_t data2;
};

} // namespace util
} // namespace p2p
#endif // P2P_KEY

