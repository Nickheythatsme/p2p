#include <memory>

#include <memory>

//
// Created by Nick Grout on 2019-08-13.
//


#ifndef HASH_HPP
#define HASH_HPP

#include <ostream>
#include <openssl/sha.h>
#include <memory>
#include <exception>
#include <cstring>
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
        hash_value = std::make_unique<unsigned char[]>(SHA_DIGEST_LENGTH);
        memcpy(hash_value.get(), rhs.hash_value.get(), SHA_DIGEST_LENGTH);
    }
    HashObject(HashObject &&rhs) noexcept:
        hash_value(std::move(rhs.hash_value))
    {
        rhs.hash_value.reset(nullptr);
        // NOTE: rhs is now broken, and any usage of it will cause a seg fault
    }
    friend std::ostream& operator<<(std::ostream& out, const HashObject& to_print)
    {
        auto previous_flags = out.flags();
        for (int i=0; i<SHA_DIGEST_LENGTH; ++i)
        {
            out << std::hex << static_cast<int>(to_print.hash_value[i]);
        }
        out.flags(previous_flags);
        return out;
    }
    int compare(const HashObject& rhs) const
    {
        return memcmp(this->hash_value.get(), rhs.hash_value.get(), SHA_DIGEST_LENGTH);
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
        hash_value = std::make_unique<unsigned char[]>(SHA_DIGEST_LENGTH);
        memcpy(hash_value.get(), rhs.hash_value.get(), SHA_DIGEST_LENGTH);
        return *this;
    }
protected:
    std::unique_ptr<unsigned char[]> hash_value {nullptr};
};

class HashBuilder
{
public:
    HashBuilder()
    {
        SHA256_Init(ctx.get());
    }
    ~HashBuilder() = default;
    int update(const void* data, size_t len)
    {
        if(finalized) {
            throw hash_exception("hash cannot be updated after being finalized");
        }
        return SHA256_Update(ctx.get(), data, len);
    }
    friend std::istream& operator>>(std::istream& in, HashBuilder& rhs)
    {
        char buff[2048];
        in.read(buff, 2048);
        rhs.update(buff, in.gcount());
        while (!in.eof() && in.good()) {
            in.read(buff, 2048);
            rhs.update(buff, in.gcount());
        }
        return in;
    }
    HashObject finalize()
    {
        if (finalized) {
            throw hash_exception("hash cannot be finalized more than once");
        }
        std::unique_ptr<unsigned char[]> md {new unsigned char[SHA_DIGEST_LENGTH]};
        SHA256_Final(md.get(), ctx.get());
        finalized = true;
        return HashObject(std::move(md));
    }
    HashBuilder& reset()
    {
        SHA256_Init(ctx.get());
        finalized = false;
        return *this;
    }
protected:
    bool finalized {false};
    std::unique_ptr<SHA256_CTX> ctx {new SHA256_CTX()};
private:
};

}

#endif //HASH_HPP
