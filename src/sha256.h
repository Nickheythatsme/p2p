//
// Created by Nick Grout on 2019-05-18.
//

// Much of this is copied from bitcoin's crytpo directory
// https://github.com/bitcoin/bitcoin/tree/master/src/crypto

#ifndef SHA256_H
#define SHA256_H

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <stdint.h>
#include <stdlib.h>
#include <cassert>
#include <openssl/sha.h>

namespace p2p
{

class shaWriter
{
public:
    shaWriter(size_t len)
    {
        // Reference:
        // https://stackoverflow.com/questions/2262386/generate-sha256-with-openssl-and-c
        /*
        assert((len > 8) && (len % 8 == 0));
        hash = (unsigned char*) malloc(len);
        SHA256_CTX sha256;
        SHA256_Init(&sha256);
        SHA256_Update(&sha256, str.c_str(), str.size());
        SHA256_Final(hash, &sha256);
        stringstream ss;
        for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
        {
            ss << hex << setw(2) << setfill('0') << (int)hash[i];
        }
         */
    }

protected:
private:
    unsigned char* hash;
};

class sha256
{
public:
    sha256()
    {
        memset(data, 0, sizeof(data));
    }
    template<typename Stream>
    Stream& serialize(Stream& s)
    {
        s.write((char*)data, sizeof(data));
        return s;
    }
    template<typename Stream>
    Stream& unserialize(Stream& s)
    {
        s.read((char*)data, sizeof(data));
        return s;
    }
    const unsigned char* start() const
    {
        return &data[0];
    }
    const unsigned char* end() const
    {
        return &data[WIDTH];
    }
    bool operator!() const
    {
        for (auto& i: data)
            if (i != 0)
                return false;
        return true;
    }
    inline int Compare(const sha256& rhs) const { return memcmp(data, rhs.data, sizeof(data)); }
    bool operator==(const sha256& rhs)
    {
        return this->Compare(rhs) == 0;
    }
    bool operator!=(const sha256& rhs)
    {
        return this->Compare(rhs);
    }
    bool operator<(const sha256& rhs)
    {
        return this->Compare(rhs) < 0;
    }
protected:
    static constexpr int WIDTH = 256 / 8;
    uint8_t data[WIDTH];
};



} // namespace p2p
#endif //SHA256_H
