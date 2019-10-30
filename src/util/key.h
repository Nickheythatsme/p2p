#include <string>
#include <memory>
#include <cstdint>
#include <fstream>
#include <random>

#ifndef P2P_KEY
#define P2P_KEY

namespace p2p {
namespace util {

class Key
{
public:
    Key();
    Key(std::mt19937_64& generator);
    Key(const Key &rhs) = default;
    Key(Key &&rhs) = default;
    ~Key() = default;
    void init(std::mt19937_64& generator);
    Key& operator=(const Key& rhs);
    std::string to_string() const;
    int compare(const Key &rhs) const;
    friend bool operator<(const Key& lhs, const Key& rhs);
    friend bool operator<=(const Key& lhs, const Key& rhs);
    friend bool operator==(const Key& lhs, const Key& rhs);
    friend bool operator!=(const Key& lhs, const Key& rhs);
    friend bool operator>(const Key& lhs, const Key& rhs);
    friend bool operator>=(const Key& lhs, const Key& rhs);
    static const size_t NLONG = 2; // two ints
private:
    uint64_t data1;
    uint64_t data2;
};

} // namespace util
} // namespace p2p
#endif // P2P_KEY

