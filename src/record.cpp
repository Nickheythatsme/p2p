#include <utility>

//
// Created by Nick Grout on 2019-08-26.
//

#include "record.h"
#include <iostream>

namespace p2p {
using namespace util;

Record::Record(UUID uuid):
    uuid(std::move(uuid))
{ }

bool operator==(const Record& lhs, const UUID& rhs)
{
    return lhs.uuid == rhs;
}

bool operator==(const Record& lhs, const Record &rhs)
{
    return lhs.uuid == rhs.uuid;
}

const UUID& Record::get_uuid() const
{
    return uuid;
}


std::ostream& Record::serialize(std::ostream& out) const
{
    out << uuid << sha256;
    writeNetworkLongLong(out, record_length);
    out.write(record_contents.get(), record_length);
    return out;
}

std::istream& Record::unserialize(std::istream& in)
{
    /*
        UUID uuid;
        Hash256 sha256; // to verify the integrity of the record
        uint64_t record_length;
        std::unique_ptr<char[]> record_contents {nullptr};
    */
    HashBuilder builder;
    in >> uuid;
    sha256.unserialize(in);
    record_length = readNetworkLongLong(in);
    record_contents.reset(new char[record_length]);
    in.read(record_contents.get(), record_length);
    return in;
}

} // namespace p2p
