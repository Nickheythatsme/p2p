//
// Created by Nick Grout on 2019-05-18.
//
#include "uuid.h"

namespace p2p {

UUID UUID::init_random()
{
    UUID _uuid;

    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<unsigned long> dis;
    _uuid._1 = dis(gen);
    _uuid._2 = dis(gen);

    // Set bits according to standard
    // Set the four leading bits of 7th byte to be 0100'B
    _uuid._1 |= 0xf000ul;
    _uuid._1 ^= 0xb000ul;

    // Set the two leading bits of 9th byte to be 10'B
    _uuid._2 |= 0x8000000000000000ul;

    return _uuid;
}

UUID UUID::parse(const char* suuid)
{
    // XXXXXXXX-xxxx-xxxx-xxxx-xxxxxxxxxxxx
    UUID u;
    u._1 = 0ul;
    int i;
    char num_1[17];
    char num_2[17];

    if (suuid[8] != '-' || suuid[13] != '-' || suuid[18] != '-' || suuid[23] != '-')
    {
        throw std::invalid_argument("Invalid uuid format: missing or misplaced '-'");
    }

    for (i=0; i<8; ++i)
    {
        num_1[i] = *suuid;
        ++suuid;
    }
    ++suuid;
    for (; i<12; ++i)
    {
        num_1[i] = *suuid;
        ++suuid;
    }
    ++suuid;
    for (; i<16; ++i)
    {
        num_1[i] = *suuid;
        ++suuid;
    }
    num_1[i] = '\0';
    ++suuid;
    for (i=0; i<4; ++i)
    {
        num_2[i] = *suuid;
        ++suuid;
    }
    ++suuid;
    for (; i<16; ++i)
    {
        num_2[i] = *suuid;
        ++suuid;
    }
    num_2[i] = '\0';

    // Check if the parsed numbers are hex
    for (i=0; i<16; ++i)
    {
        if (!isxdigit(num_1[i])) {
            throw std::invalid_argument(std::string("UUID had non-hex value where it should have had one: ") + num_1[i]);
        }
        if (!isxdigit(num_2[i])) {
            throw std::invalid_argument(std::string("UUID had non-hex value where it should have had one: ") + num_2[i]);
        }
    }

    UUID uuid;

    uuid._1 = strtoul(num_1, nullptr, 16);
    uuid._2 = strtoul(num_2, nullptr, 16);

    // Check bits
    auto corrected_uuid = uuid._1 | 0xf000ul;
    corrected_uuid ^= 0xb000ul;
    if (corrected_uuid != uuid._1) {
        throw std::invalid_argument("Leading 4 bits of 7th byte did not equal 0100'B");
    }

    if ((uuid._2 |= 0x8000000000000000ul) != uuid._2)
    {
        throw std::invalid_argument("Leading 2 bits of 9th byte did not equal 10'B");
    }

    return uuid;
}

std::string UUID::to_string() const
{
    char suuid[37];
#if defined(__BYTE_ORDER__)&&(__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
    auto current = (unsigned char*) &_1;
    current += 7; // Move to the last byte of the end of the number
    int i;
    // XXXXXXXX-xxxx-xxxx-xxxx-xxxxxxxxxxxx
    for (i=0; i<8;i += 2)
    {
        sprintf(&suuid[i], "%.2x", *current);
        --current;
    }
    // xxxxxxxx-XXXX-xxxx-xxxx-xxxxxxxxxxxx
    suuid[i++] = '-';
    for (int j=0; j<4; j+=2, i += 2)
    {
        sprintf(&suuid[i], "%.2x", *current);
        --current;
    }
    // xxxxxxxx-xxxx-XXXX-xxxx-xxxxxxxxxxxx
    suuid[i++] = '-';
    for (int j=0; j<4; j+=2, i += 2)
    {
        sprintf(&suuid[i], "%.2x", *current);
        --current;
    }
    // Move to the second integer
    current = (unsigned char*) &_2;
    current += 7; // Move to the last byte of the end of the number
    // xxxxxxxx-xxxx-xxxx-XXXX-xxxxxxxxxxxx
    suuid[i++] = '-';
    for (int j=0; j<4; j+=2, i += 2)
    {
        sprintf(&suuid[i], "%.2x", *current);
        --current;
    }
    // xxxxxxxx-xxxx-xxxx-xxxx-XXXXXXXXXXXX
    suuid[i++] = '-';
    for (int j=0; j<12; j+=2, i += 2)
    {
        sprintf(&suuid[i], "%.2x", *current);
        --current;
    }
    suuid[i] = '\0';
#elif defined(__BYTE_ORDER__)&&(__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
    auto current = (unsigned char*) &_1;
    int i;
    // XXXXXXXX-xxxx-xxxx-xxxx-xxxxxxxxxxxx
    for (i=0; i<8;i += 2)
    {
        sprintf(&suuid[i], "%.2x", *current);
        ++current;
    }
    // xxxxxxxx-XXXX-xxxx-xxxx-xxxxxxxxxxxx
    suuid[i++] = '-';
    for (int j=0; j<4; j+=2, i += 2)
    {
        sprintf(&suuid[i], "%.2x", *current);
        ++current;
    }
    // xxxxxxxx-xxxx-XXXX-xxxx-xxxxxxxxxxxx
    suuid[i++] = '-';
    for (int j=0; j<4; j+=2, i += 2)
    {
        sprintf(&suuid[i], "%.2x", *current);
        ++current;
    }
    // Move to the second integer
    current = (unsigned char*) &_2;
    current += 7; // Move to the last byte of the end of the number
    // xxxxxxxx-xxxx-xxxx-XXXX-xxxxxxxxxxxx
    suuid[i++] = '-';
    for (int j=0; j<4; j+=2, i += 2)
    {
        sprintf(&suuid[i], "%.2x", *current);
        ++current;
    }
    // xxxxxxxx-xxxx-xxxx-xxxx-XXXXXXXXXXXX
    suuid[i++] = '-';
    for (int j=0; j<12; j+=2, i += 2)
    {
        sprintf(&suuid[i], "%.2x", *current);
        ++current;
    }
    suuid[i] = '\0';
#endif
    return std::string(suuid);
}

// Displayed as
// XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX
std::ostream& operator<<(std::ostream& out, const UUID& u)
{
    out << u.to_string();
    return out;
}

UUID& UUID::operator=(const char* str)
{
    auto parsed = UUID::parse(str);
    return this->operator=(parsed);
}

UUID& UUID::operator=(const UUID& uuid)
{
    _1 = uuid._1;
    _2 = uuid._2;
    return *this;
}

// Equality
bool UUID::operator==(const UUID& rhs) const
{
    return _1 == rhs._1 && _2 == rhs._2;
}
bool UUID::operator==(const char* str) const
{
    return this->to_string() == str;
}

} // namespace p2p

