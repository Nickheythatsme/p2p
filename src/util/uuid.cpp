//
// Created by Nick Grout on 2019-05-18.
//
// A uuid is a globally unique 36 character string. 
// This implementation is pretty wonky in that we use two 8 byte integers to hold
// the uuid. I thought there would be a performance benefit to using integers,
// however I have not tested this and a single 16 byte array would probably have
// identical performance.

#include "uuid.h"
#include <chrono>
#include <cstdio>
#include <iomanip>
#include <sstream>

namespace p2p::util {

UUID::UUID():
    data(new uchar[UUID_NBYTES])
{ }

UUID::UUID(const UUID &rhs):
    data(new uchar[UUID_NBYTES])
{
    memcpy(data.get(), rhs.data.get(), UUID_NBYTES);
}

/*
 * Generate a random uuid, of 16 bytes (128 bits) according to RFC 4122 section 4.4.
 * Referenced from https://www.cryptosys.net/pki/uuid-rfc4122.html
 */
UUID UUID::init_random()
{
    auto now = std::chrono::steady_clock::now();
    const std::chrono::duration since_epoch = now.time_since_epoch();
    auto now_cast = std::chrono::duration_cast<std::chrono::milliseconds>(since_epoch).count();
    std::seed_seq seed {now_cast};
    std::mt19937_64 gen (seed);
    return UUID::init_random(gen);
}

UUID UUID::init_random(std::mt19937_64& gen)
{
    auto uuid = new uchar[UUID_NBYTES];
    auto i_ptr = reinterpret_cast<uint64_t *>(uuid);
    *i_ptr = gen();
    ++i_ptr;
    *i_ptr = gen();

    uuid[6] = 0x40ul | (uuid[6] & 0xful);
    uuid[8] = 0x80ul | (uuid[8] & 0x3ful);

    return UUID(uuid);
}

UUID::UUID(unsigned char* data):
    data(new uchar[UUID_NBYTES])
{
    memcpy(this->data.get(), data, UUID_NBYTES);
}

UUID::UUID(std::unique_ptr<uchar[]> &&data):
    data(std::move(data))
{ }

UUID UUID::parse (const char *suuid)
{
    // XXXXXXXX-xxxx-xxxx-xxxx-xxxxxxxxxxxx
    if (suuid[8] != '-' || suuid[13] != '-' || suuid[18] != '-' || suuid[23] != '-')
    {
        throw std::invalid_argument ("Invalid uuid format: missing or misplaced '-'");
    }

    if (suuid[14] != '4')
    {
        throw std::invalid_argument ("Leading 4 bits of 7th byte did not equal 0100'B");
    }
    for (int i = 0; i < 36; ++i)
    {
        if (!isxdigit (static_cast<int>(suuid[i])) && suuid[i] != '-')
        {
            throw std::invalid_argument("UUID had unexpected non-hex value");
        }
    }

    /*
    char suuid_clean[32];
    memcpy(suuid_clean, suuid, 8);
    memcpy(&suuid_clean[8],  &suuid[9], 4);
    memcpy(&suuid_clean[12], &suuid[14], 4);
    memcpy(&suuid_clean[16], &suuid[19], 4);
    memcpy(&suuid_clean[20], &suuid[24], 12);

    // Check if the numbers are hex
    for (int i = 0; i < UUID_NBYTES; ++i)
    {
        if (!isxdigit (static_cast<int>(suuid_clean[i])))
        {
            throw std::invalid_argument("UUID had unexpected non-hex value");
        }
    }
    */

    auto uuid = new uchar[UUID_NBYTES];
    char* current;
    unsigned long p1 = 0x0;
    p1 = strtoul(suuid, &current, 16);
    p1 = (p1 << 16);
    ++current;
    p1 += strtoul(current, &current, 16);
    p1 = (p1 << 16);
    ++current;
    p1 += strtoul(current, &current, 16);
    ++current;

    unsigned long p2 = 0x0;
    p2 = strtoul(current, &current, 16);
    p2 = (p2 << 48);
    ++current;
    p2 += strtoul(current, &current, 16);

    p1 = be64toh(p1);
    memcpy(uuid, reinterpret_cast<const void*>(&p1), sizeof(p1));
    p2 = be64toh(p2);
    memcpy(&uuid[8], reinterpret_cast<const void*>(&p2), sizeof(p2));

    /*
    if ((uuid[8] ^ 0x80u) != 0)
    {
        throw std::invalid_argument ("Leading 2 bits of 9th byte did not equal 10'B");
    }
    */

    return UUID(uuid);
}

std::string UUID::to_string () const
{
    std::stringstream ss;
    auto current = data.get();
    // XXXXXXXX-xxxx-xxxx-xxxx-xxxxxxxxxxxx
    for (int i = 0; i < 4; ++i)
    {
        ss << std::setw(2) << std::setfill('0') << std::hex << (int)*current;
        ++current;
    }
    // xxxxxxxx-XXXX-xxxx-xxxx-xxxxxxxxxxxx
    ss << '-';
    for (int i = 0; i < 2; ++i)
    {
        ss << std::setw(2) << std::setfill('0') << std::hex << (int)*current;
        ++current;
    }
    // xxxxxxxx-xxxx-XXXX-xxxx-xxxxxxxxxxxx
    ss << '-';
    for (int i = 0; i < 2; ++i)
    {
        ss << std::setw(2) << std::setfill('0') << std::hex << (int)*current;
        ++current;
    }
    // xxxxxxxx-xxxx-xxxx-XXXX-xxxxxxxxxxxx
    ss << '-';
    for (int i = 0; i < 2; ++i)
    {
        ss << std::setw(2) << std::setfill('0') << std::hex << (int)*current;
        ++current;
    }
    // xxxxxxxx-xxxx-xxxx-xxxx-XXXXXXXXXXXX
    ss << '-';
    for (int i = 0; i < 6; ++i)
    {
        ss << std::setw(2) << std::setfill('0') << std::hex << (int)*current;
        ++current;
    }
    return ss.str();
}

// Displayed as
// XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX
std::ostream &operator<<(std::ostream &out, const UUID &u)
{
    out << u.to_string ();
    return out;
}

UUID &UUID::operator= (const UUID &rhs)
{
    memcpy(data.get(), rhs.data.get(), UUID_NBYTES);
    return *this;
}

// Equality operators
bool UUID::operator==(const UUID &rhs) const
{
    return memcmp(data.get(), rhs.data.get(), UUID_NBYTES) == 0;
}

} // namespace p2p::util

