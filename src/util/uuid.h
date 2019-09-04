/*
 * Nicholas Grout 8/27
 *
 * Warning: This implementation is in absolutely no way endian agnostic.
 * It only works on little endian as of now.
 */
#ifndef P2P_UUID_H
#define P2P_UUID_H

#include <ostream>
#include <memory>
#include <cstring>
#include <random>
#include <string>
#include <exception>
#include <functional>

#if !defined(HAVE_DECL_BE64TOH) || HAVE_DECL_BE64TOH == 0
    #include "../crypto/endian.h"
    #undef HAVE_DECL_BE64TOH
    #define HAVE_DECL_BE64TOH 0
#endif

#define UUID_NBYTES 16

namespace p2p {
namespace util {

using uchar = unsigned char;

// 128 bit number
class UUID
{
    public:
        explicit UUID(std::unique_ptr<uchar[]> &&data);
        UUID(UUID &&rhs) noexcept = default;
        UUID(const UUID &rhs);

        // Initialize the UUID
        static UUID init_random();
        static UUID init_random(std::mt19937_64& generator);

        // Parse the UUID from a human readable string and back again
        static UUID parse(const char *suuid);
        std::string to_string() const;
        friend std::ostream &operator<<(std::ostream &out, const UUID &u);

        // Assignment
        UUID &operator=(const UUID &uuid);

        // Equality
        bool operator==(const UUID &rhs) const;

        // Mod operator
        uint64_t operator%(uint64_t max) const;
        friend struct std::hash<UUID>;
    protected:
    private:
        UUID();
        std::unique_ptr<uchar[]> data {nullptr};
};


} // namespace util
} // namespace p2p

namespace std
{
template<> struct hash<p2p::util::UUID>
{
    typedef p2p::util::UUID argument_type;
    typedef std::size_t result_type;
    result_type operator()(argument_type const& s) const noexcept
    {
        auto d1 = reinterpret_cast<unsigned long*>(s.data.get());
        auto d2 = reinterpret_cast<unsigned long*>(&s.data.get()[8]);
        result_type const h1 ( std::hash<unsigned long>{}(*d1) );
        result_type const h2 ( std::hash<unsigned long>{}(*d2) );
        return h1 ^ (h2 << 0x1);
    }
};
}

#endif // P2P_UUID_H
