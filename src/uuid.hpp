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
    char suuid[37];
    char* current = (char*) &u._1 + 0x4;
    for (int i=0; i<8; ++i)
    {
        sprintf(&suuid[i], "%x", *current);
        ++i;
        --current;
    }
    suuid[36] = '\0';
    out << suuid;

    #if defined(__BYTE_ORDER__)&&(__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
    // TODO
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
