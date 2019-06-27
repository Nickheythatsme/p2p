#ifndef P2P_UUID_H
#define P2P_UUID_H

#include <cstdio>
#include <ostream>
#include <random>
#include <string>
#include <exception>

namespace p2p {

// 128 bit number
class UUID
{
public:
    UUID(const char* suuid);
    UUID(UUID&& rhs) = default;
    UUID(const UUID& rhs) = default;

    static UUID init_random();
    static UUID parse(const char* suuid);
    std::string to_string() const;
    friend std::ostream& operator<<(std::ostream& out, const UUID& u);

    // Assignment
    UUID& operator=(const char* str);
    UUID& operator=(const UUID& uuid);

    // Equality
    bool operator==(const UUID& rhs) const;
    bool operator==(const char* str) const;
protected:
    unsigned long _1;
    unsigned long _2;
private:
    UUID() = default;
};

#endif // P2P_UUID_H
} // namespace p2p
