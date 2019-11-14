#include <utility>

//
// Created by Nick Grout on 2019-08-26.
//

#include "record.h"
#include <iostream>

namespace p2p {
using namespace util;
using namespace networking;

Record Record::build(std::string record_contents)
{
    auto uuid = UUID::init_random();
    HashBuilder builder;
    builder.write(reinterpret_cast<const unsigned char*>(record_contents.c_str()), record_contents.size());
    return Record(
            std::move(uuid),
            std::move(builder.finalize()),
            std::move(record_contents)
            );
}

Record::Record(UUID uuid, Hash256 sha256, std::string record_contents) :
    uuid(std::move(uuid)),
    hash256(std::move(sha256)),
    record_contents(std::move(record_contents))
{
}

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

const Hash256& Record::get_hash256() const
{
    return hash256;
}

const std::string& Record::get_record_contents() const
{
    return record_contents;
}

std::ostream& Record::serialize(std::ostream& out) const
{
    out << uuid << hash256;
    uint64_t record_length = (uint64_t) record_contents.size();
    writeNetworkLongLong(out, record_length);
    out.write(record_contents.c_str(), record_length);
    return out;
}

std::istream& Record::unserialize(std::istream& in)
{
    char buff[SERIALIZE_BUFF_SIZE];
    uint64_t record_length;

    HashBuilder builder;
    in >> uuid;
    hash256.unserialize(in);

    record_length = readNetworkLongLong(in);
    record_contents.clear();
    record_contents.reserve(record_length + 1);
    // TODO add iterative reading, so that large files aren't read all at once.
    in.read(buff, record_length);
    record_contents.append(buff, record_length);
    builder.write(reinterpret_cast<unsigned char*>(buff), record_length);
    if (builder.finalize() != hash256) {
        throw serialize_exception(SERIALIZE_EXCEPTION_HASH);
    }
    return in;
}

} // namespace p2p
