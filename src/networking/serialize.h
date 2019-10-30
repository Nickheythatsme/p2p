#include <cstdint>
#include <arpa/inet.h>
#include <fstream>

#ifndef P2P_SERIALIZE
#define P2P_SERIALIZE

#define SERIALIZE_BUFF_SIZE 2048

namespace p2p { namespace networking {

#define SERIALIZE_EXCEPTION_HASH "Provided hash code did not match the one calculated during serialization"

class serialize_exception : public std::exception
{
    public:
        explicit serialize_exception(const char *what_arg) :
        std::exception(),
        what_arg(what_arg)
        {
        }
        const char *what() const noexcept override
        {
            return what_arg;
        }
        serialize_exception& operator=(const exception &other) noexcept
        {
            what_arg = other.what();
            return *this;
        }
    private:
        const char *what_arg;
};
 
inline uint64_t readNetworkLongLong(std::istream& istr)
{
    uint64_t network_value = 0;
    uint64_t value = 0;
    istr.read((char*)&network_value, sizeof(network_value));
    value = ntohl((uint32_t) network_value);
    value <<= 32;
    network_value >>= 32;
    value += (uint32_t) ntohl((uint32_t) network_value);
    return value;
}

inline uint32_t readNetworkLong(std::istream& istr)
{
    uint32_t value = 0;
    istr.read((char*)&value, sizeof(value));
    return ntohl(value);
}

inline uint16_t readNetworkShort(std::istream& istr)
{
    uint16_t value = 0;
    istr.read((char*)&value, sizeof(value));
    return ntohs(value);
}

inline std::ostream& writeNetworkLongLong(std::ostream& ostr, uint64_t value)
{
    uint64_t network_value = 0;
    network_value = htonl(value);
    network_value <<= 32;
    value >>= 32;
    network_value += htonl(value);
    ostr.write((char*)&network_value, sizeof(network_value));
    return ostr;
}

inline std::ostream& writeNetworkLong(std::ostream& ostr, uint32_t value)
{
    value = htonl(value);
    ostr.write((char*)&value, sizeof(value));
    return ostr;
}

inline std::ostream& writeNetworkShort(std::ostream& ostr, uint16_t value)
{
    value = htons(value);
    ostr.write((char*)&value, sizeof(value));
    return ostr;
}

class Serializable {
    public:
        virtual std::ostream& serialize(std::ostream& out) const = 0;
        virtual std::istream& unserialize(std::istream& in) = 0;
};

} // namespace networking
} // namespace p2p


#endif
