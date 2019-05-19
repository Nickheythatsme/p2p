//
// Created by Nick Grout on 2019-05-18.
//
#include "HashCode.h"

namespace p2p {

template<typename Stream>
Stream& HashCode::serialize(Stream& s)
{
    s.write((char*)data, sizeof(data));
    return s;
}

template<typename Stream>
Stream& HashCode::unserialize(Stream& s)
{
    s.read((char*)data, sizeof(data));
    return s;
}

const unsigned char* HashCode::start() const
{
    return &data[0];
}

const unsigned char* HashCode::end() const
{
    return &data[WIDTH];
}

bool HashCode::operator!() const
{
    for (auto& i: data)
        if (i != 0)
            return false;
    return true;
}

inline int HashCode::Compare(const HashCode& rhs) const
{
    return memcmp(data, rhs.data, sizeof(data));
}

bool HashCode::operator==(const HashCode& rhs)
{
    return this->Compare(rhs) == 0;
}

bool HashCode::operator<(const HashCode& rhs)
{
    return this->Compare(rhs) < 0;
}

// Set the data inside the hash
HashCode HashCode::set(const uint8_t* new_data)
{
    for (int i=0; i<WIDTH; ++i)
    {
        data[i] = new_data[i];
    }
}

HashWriter::HashWriter()
{
    // Reference:
    // https://stackoverflow.com/questions/2262386/generate-sha256-with-openssl-and-c
    SHA256_Init(&sha256);
}

void HashWriter::write(const char* data, size_t len)
{
    SHA256_Update(&sha256, data, len);
}

HashCode HashWriter::finalize()
{
    HashCode hashCode;
    unsigned char data[256];
    SHA256_Final(data, &sha256);
    return hashCode.set((uint8_t*) data);
}

}
