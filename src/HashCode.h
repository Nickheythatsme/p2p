//
// Created by Nick Grout on 2019-05-18.
//

// Much of this is copied from bitcoin's crytpo directory
// https://github.com/bitcoin/bitcoin/tree/master/src/crypto

#ifndef HASHCODE_H
#define HASHCODE_H

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <stdint.h>
#include <stdlib.h>
#include <cassert>
#include <openssl/sha.h>

namespace p2p
{

class HashCode
{
public:
    HashCode() = default;
    template<typename Stream>
    Stream& serialize(Stream& s);
    template<typename Stream>
    Stream& unserialize(Stream& s);
    const unsigned char* start() const;
    const unsigned char* end() const;
    bool operator!() const;
    // Equality/comparison
    inline int Compare(const HashCode& rhs) const;
    bool operator==(const HashCode& rhs);
    bool operator<(const HashCode& rhs);
    // Set the data inside the hash
    HashCode set(const uint8_t* new_data);
protected:
    static constexpr int WIDTH = 256 / 8;
    uint8_t data[WIDTH];
};

class HashWriter
{
public:
    HashWriter();
    void write(const char* data, size_t len);
    HashCode finalize();
protected:
private:
    SHA256_CTX sha256 {0};
};


} // namespace p2p
#endif //HASHCODE_H
