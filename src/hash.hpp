//
// Created by Nick Grout on 2019-08-13.
//

#ifndef HASH_HPP
#define HASH_HPP

#include "crypto/sha256.h"
#include "crypto/common.h"
#include <ostream>
#include <memory>
#include <exception>
#include <cstring>

// TODO remove stdio
#include <cstdio>

namespace p2p {

class hash_exception: public std::exception
{
public:
    explicit hash_exception(const char* what_arg):
        std::exception(),
        what_arg(what_arg)
    { }
    const char* what() const noexcept override
    {
        return what_arg;
    }
    hash_exception& operator=(const exception& other) noexcept
    {
        what_arg = other.what();
        return *this;
    }
private:
    const char* what_arg;
};

class HashObject
{
public:
    explicit HashObject(std::unique_ptr<unsigned char[]>&& hash_value):
        hash_value(std::move(hash_value))
    { }
    HashObject(const HashObject &rhs)
    {
        hash_value = std::make_unique<unsigned char[]>(CSHA256::OUTPUT_SIZE);
        memcpy(hash_value.get(), rhs.hash_value.get(), CSHA256::OUTPUT_SIZE);
    }
    // NOTE: this breaks rhs, any usage of it after this call will cause a seg fault
    HashObject(HashObject &&rhs) noexcept:
        hash_value(std::move(rhs.hash_value))
    {
        rhs.hash_value.reset(nullptr);
    }
    friend std::ostream& operator<<(std::ostream& out, const HashObject& to_print)
    {
        auto previous_flags = out.flags();
        auto val = static_cast<unsigned char*>(to_print.hash_value.get());
        for (int i=0; i<CSHA256::OUTPUT_SIZE / 8; i++)
        {
            out << std::hex << ReadBE64(val);
            val += sizeof(uint64_t);
        }
        out.flags(previous_flags);
        return out;
    }
    int compare(const HashObject& rhs) const
    {
        return memcmp(this->hash_value.get(), rhs.hash_value.get(), CSHA256::OUTPUT_SIZE);
    }
    friend bool operator==(const HashObject& lhs, const HashObject& rhs)
    {
        return lhs.compare(rhs) == 0;
    }
    friend bool operator!=(const HashObject& lhs, const HashObject& rhs)
    {
        return lhs.compare(rhs) != 0;
    }
    friend bool operator<(const HashObject& lhs, const HashObject& rhs)
    {
        return lhs.compare(rhs) < 0;
    }
    friend bool operator>(const HashObject& lhs, const HashObject& rhs)
    {
        return lhs.compare(rhs) > 0;
    }
    friend bool operator<=(const HashObject& lhs, const HashObject& rhs)
    {
        return lhs.compare(rhs) <= 0;
    }
    friend bool operator>=(const HashObject& lhs, const HashObject& rhs)
    {
        return lhs.compare(rhs) >= 0;
    }
    HashObject& operator=(const HashObject& rhs)
    {
        hash_value = std::make_unique<unsigned char[]>(CSHA256::OUTPUT_SIZE);
        memcpy(hash_value.get(), rhs.hash_value.get(), CSHA256::OUTPUT_SIZE);
        return *this;
    }
    int operator%(int val) const
    {
        return static_cast<uint32_t>(*hash_value.get()) % val;
    }
protected:
    std::unique_ptr<unsigned char[]> hash_value {nullptr};
};

class HashBuilder
{
public:
    HashBuilder():
        csha256()
    { }
    ~HashBuilder() = default;
    HashBuilder& write(const unsigned char* data, size_t len)
    {
        if(finalized) {
            throw hash_exception("hash cannot be updated after being finalized");
        }
        csha256.Write(data, len);
        return *this;
    }
    friend std::istream& operator>>(std::istream& in, HashBuilder& rhs)
    {
        unsigned char buff[2048];
        in.read(reinterpret_cast<char*>(buff), 2048);
        rhs.write(buff, in.gcount());
        while (!in.eof() && in.good()) {
            in.read(reinterpret_cast<char*>(buff), 2048);
            rhs.write(buff, in.gcount());
        }
        return in;
    }
    HashObject finalize()
    {
        if (finalized) {
            throw hash_exception("hash cannot be finalized more than once");
        }
        std::unique_ptr<unsigned char[]> md {new unsigned char[CSHA256::OUTPUT_SIZE]};
        csha256.Finalize(md.get());
        finalized = true;
        return HashObject(std::move(md));
    }
    HashBuilder& reset()
    {
        finalized = false;
        csha256.Reset();
        return *this;
    }
protected:
    bool finalized {false};
    CSHA256 csha256;
private:
};

}

#endif //HASH_HPP
