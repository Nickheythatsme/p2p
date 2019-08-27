#ifndef P2P_UUID_H
#define P2P_UUID_H

#include <ostream>
#include <memory>
#include <cstring>
#include <random>
#include <string>
#include <exception>

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
        UUID(std::unique_ptr<uchar[]> &&data);
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
    protected:
    private:
        UUID();
        explicit UUID(unsigned char* data);
        std::unique_ptr<uchar[]> data {nullptr};
};

} // namespace util
} // namespace p2p

#endif // P2P_UUID_H
