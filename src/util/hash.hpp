#include <memory>

//
// Created by Nick Grout on 2019-08-13.
//

#ifndef HASH_HPP
#define HASH_HPP

#include <iostream>
#include <memory>
#include <exception>
#include <cstring>
#include "../crypto/sha256.h"

#if !defined(HAVE_DECL_BE64TOH) || HAVE_DECL_BE64TOH == 0
#include "../crypto/endian.h"
#define HAVE_DECL_BE64TOH 0
#endif

namespace p2p {
namespace util
{

class hash_exception : public std::exception
{
    public:
        explicit hash_exception(const char *what_arg) :
        std::exception(),
        what_arg(what_arg)
        {
        }
        const char *what() const noexcept override
        {
            return what_arg;
        }
        hash_exception &operator=(const exception &other) noexcept
        {
            what_arg = other.what();
            return *this;
        }
    private:
        const char *what_arg;
};

class Hash
{
    public:
        explicit Hash(std::unique_ptr<unsigned char[]> &&hash_value) :
        hash_value(nullptr)
        {
            this->hash_value.reset(reinterpret_cast<uint64_t *>(hash_value.release()));
            for(int i = 0; i < Hash::OUTPUT_SIZE; ++i) {
                auto be = this->hash_value.get()[i];
                this->hash_value.get()[i] = be64toh(be);
            }
        }
        Hash(const Hash &rhs)
        {
            memcpy(hash_value.get(), rhs.hash_value.get(), CSHA256::OUTPUT_SIZE);
        }
        Hash(Hash &&rhs) noexcept:
            hash_value(std::move(rhs.hash_value))
        {
            rhs.hash_value = std::make_unique<uint64_t[]>(Hash::OUTPUT_SIZE);
        }
        friend std::ostream &operator<<(std::ostream &out, const Hash &to_print)
        {
            auto previous_flags = out.flags();
            for(int i = 0; i < Hash::OUTPUT_SIZE; ++i) {
                out << std::hex << to_print.hash_value.get()[i];
            }
            out.flags(previous_flags);
            return out;
        }
        // Returns the memcmp of the two values, 0 means equal
        int compare(const Hash &rhs) const
        {
            return memcmp(this->hash_value.get(), rhs.hash_value.get(), CSHA256::OUTPUT_SIZE);
        }
        friend bool operator==(const Hash &lhs, const Hash &rhs)
        {
            return lhs.compare(rhs) == 0;
        }
        friend bool operator!=(const Hash &lhs, const Hash &rhs)
        {
            return lhs.compare(rhs) != 0;
        }
        friend bool operator<(const Hash &lhs, const Hash &rhs)
        {
            return lhs.compare(rhs) < 0;
        }
        friend bool operator>(const Hash &lhs, const Hash &rhs)
        {
            return lhs.compare(rhs) > 0;
        }
        friend bool operator<=(const Hash &lhs, const Hash &rhs)
        {
            return lhs.compare(rhs) <= 0;
        }
        friend bool operator>=(const Hash &lhs, const Hash &rhs)
        {
            return lhs.compare(rhs) >= 0;
        }
        Hash &operator=(const Hash &rhs)
        {
            memcpy(hash_value.get(), rhs.hash_value.get(), CSHA256::OUTPUT_SIZE);
            return *this;
        }
        int operator%(int val) const
        {
            int acc = 0;
            acc += hash_value.get()[0] % val;
            acc += hash_value.get()[1] % val;
            acc += hash_value.get()[2] % val;
            acc += hash_value.get()[3] % val;
            return acc % val;
        }
        static const size_t OUTPUT_SIZE = 4;
    protected:
        std::unique_ptr<uint64_t[]> hash_value{new uint64_t[Hash::OUTPUT_SIZE]};
};

class HashBuilder
{
    public:
        HashBuilder() :
        csha256()
        {
        }
        ~HashBuilder() = default;
        HashBuilder &write(const unsigned char *data, size_t len)
        {
            if(finalized) {
                throw hash_exception("hash cannot be updated after being finalized");
            }
            csha256.Write(data, len);
            return *this;
        }
        friend std::istream &operator>>(std::istream &in, HashBuilder &rhs)
        {
            unsigned char buff[2048];
            in.read(reinterpret_cast<char *>(buff), 2048);
            rhs.write(buff, in.gcount());
            while(!in.eof() && in.good()) {
                in.read(reinterpret_cast<char *>(buff), 2048);
                rhs.write(buff, in.gcount());
            }
            return in;
        }
        Hash finalize()
        {
            if(finalized) {
                throw hash_exception("hash cannot be finalized more than once");
            }
            std::unique_ptr<unsigned char[]> md{new unsigned char[CSHA256::OUTPUT_SIZE]};
            csha256.Finalize(md.get());
            finalized = true;
            return Hash(std::move(md));
        }
        HashBuilder &reset()
        {
            finalized = false;
            csha256.Reset();
            return *this;
        }
    protected:
        bool finalized{false};
        CSHA256 csha256;
    private:
};

} // namespace util
} // namespace p2p

#endif //HASH_HPP
