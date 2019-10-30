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
    auto record_key = Key::init_random();
    HashBuilder builder;
    builder.write(reinterpret_cast<const unsigned char*>(record_contents.c_str()), record_contents.size());
    return Record(
            std::move(record_key),
            std::move(builder.finalize()),
            std::move(record_contents)
            );
}

Record::Record(Key record_key, Hash256 sha256, std::string record_contents) :
    record_key(std::move(record_key)),
    sha256(std::move(sha256)),
    record_contents(std::move(record_contents))
{
}

bool operator==(const Record& lhs, const Key& rhs)
{
    return lhs.record_key == rhs;
}

bool operator==(const Record& lhs, const Record &rhs)
{
    return lhs.record_key == rhs.record_key;
}

const Key& Record::get_record_key() const
{
    return record_key;
}

const Hash256& Record::get_hash256() const
{
    return sha256;
}

const std::string& Record::get_record_contents() const
{
    return record_contents;
}

std::ostream& Record::serialize(std::ostream& out) const
{
    out << record_key << sha256;
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
    in >> record_key;
    sha256.unserialize(in);

    record_length = readNetworkLongLong(in);
    record_contents.clear();
    record_contents.reserve(record_length + 1);
    // TODO add iterative reading, so that large files aren't read all at once.
    in.read(buff, record_length);
    record_contents.append(buff, record_length);
    builder.write(reinterpret_cast<unsigned char*>(buff), record_length);
    if (builder.finalize() != sha256) {
        throw serialize_exception(SERIALIZE_EXCEPTION_HASH);
    }
    return in;
}

} // namespace p2p
