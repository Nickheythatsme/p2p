//
// Created by Nick Grout on 2019-08-13.
//


#ifndef HASH_HPP
#define HASH_HPP

#include <ostream>
#include <openssl/sha.h>
#include <memory>

namespace p2p {

class BaseHash
{
public:
    BaseHash() = default;
    virtual ~BaseHash() = default;
    virtual int update(const void* data, size_t len) = 0;
    virtual std::unique_ptr<unsigned char> final() = 0;
protected:
private:
};

class Hash256: public BaseHash
{
public:
    Hash256(): BaseHash()
    {
        SHA256_Init(ctx.get());
    }
    ~Hash256() = default;
    int update(const void* data, size_t len) override
    {
        return SHA256_Update(ctx.get(), data, len);
    }
    std::unique_ptr<unsigned char> final() override
    {
        std::unique_ptr<unsigned char> md {new unsigned char[SHA_DIGEST_LENGTH]};
        SHA256_Final(md.get(), ctx.get());
        return md;
    }
protected:
    std::unique_ptr<SHA256_CTX> ctx {new SHA256_CTX()};

private:
};

}

#endif //HASH_HPP
