//
// Created by Nick Grout on 2019-08-13.
//

#ifndef HASH_HPP
#define HASH_HPP

#include <ostream>
#include <string>
#define MAX_BUFFSIZE 1024



namespace p2p {

extern "C" uint32_t SuperFastHash(const char * data, u_int64_t len);

class BaseHash
{
public:
    virtual ~BaseHash() = default;
    virtual void write(std::istream& new_contents) = 0;
    virtual void write(const char* new_contents, size_t len) = 0;
    virtual std::ostream& finalize(std::ostream& out) = 0;
protected:
private:
};

class Hash256: public BaseHash
{
public:
    Hash256() = default;
    ~Hash256() = default;
    void write(std::istream& new_contents) override
    {
        char buff[MAX_BUFFSIZE];
        new_contents.peek();

        while(!new_contents.eof()) {
            new_contents.read(buff, MAX_BUFFSIZE);
            auto len = new_contents.gcount();
            this->write(buff, len);
        }
    }
    void write(const char* new_contents, size_t len) override
    {
        size_t i = 0, increment = len/4, remainder = len % 4;

        for (; i < len - remainder; i += increment) {
            hash[index] ^= SuperFastHash(&new_contents[i], increment);
            ++index %= 8;
        }

        hash[index] ^= SuperFastHash(&new_contents[i], remainder);
        ++index %= 8;
    }
    std::ostream& finalize(std::ostream& out) override
    {
        for (const auto& hash_value : hash) {
            out << std::hex << hash_value;
        }
        return out;
    }
    std::string finalize()
    {
        std::stringstream ss;
        finalize(ss);
        return ss.str();
    }
    friend bool operator==(const Hash256& lhs, const Hash256& rhs)
    {
        return memcmp(lhs.hash, rhs.hash, sizeof(rhs.hash));
    }
protected:
private:
    uint32_t hash[8] {0};
    int index {0};
};

}

#endif //HASH_HPP
