#ifndef P2P_UUID_H
#define P2P_UUID_H

#include <cstdio>
#include <iostream>

namespace p2p {

// 128 bit number
class uuid
{
    public:
        uuid()
        {
            _1 = 0x12345678;
            _2 = 0x9abcdef0;
        }
        ~uuid()
        {
        }
        friend std::ostream& operator<<(std::ostream& out, const uuid& u);
    private:
        unsigned long _1;
        unsigned long _2;
};


// Displayed as
// XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX
std::ostream& operator<<(std::ostream& out, const uuid& u)
{
    #if defined(__BYTE_ORDER__)&&(__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
    // Written for little endian
    unsigned short* _2a = (unsigned short*)&u._2;
    unsigned short* _2b = (unsigned short*)(&u._2 + 0x1);
    out << "sizeof(unsigned short) = " << sizeof(unsigned short) << std::endl;
    out << _2a << std::endl;
    out << _2b << std::endl;
    out << std::hex << u._1 << '-' << std::hex << *_2a << '-' << std::hex << *_2b;
    #elif defined(__BYTE_ORDER__)&&(__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
    char *current_char = (char*)&_1;
    for (int i=0; i<4; ++i)
    {
        printf("%x", *current_char);
        ++current_char;
    }
    #endif
    return out;
}


#endif // P2P_UUID_H
} // namespace p2p
