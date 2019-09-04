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

LocalRecord::LocalRecord(UUID uuid, Hash256 sha256, const char *filename):
    Record(std::move(uuid)),
    sha256(std::move(sha256)),
    filename(filename)
{ }

LocalRecord LocalRecord::CreateLocalRecord(const char* filename)
{
    HashBuilder builder;
    std::ifstream fin(filename);
    if (!fin) {
        throw record_exception("file does not exist");
    }
    fin >> builder;
    return LocalRecord(UUID::init_random(), builder.finalize(), filename);
}

bool LocalRecord::is_remote() const
{
    return false;
}

std::ostream &LocalRecord::retrieve(std::ostream &out)
{
    std::ifstream in(filename);
    if (!in) {
        throw record_exception("file does not exist");
    }
    char buff[2048];
    HashBuilder builder;
    in.read(buff, 2048);
    auto last_read = in.gcount();
    out.write(buff, last_read);
    builder.write(reinterpret_cast<unsigned char*>(buff), last_read);
    while (!in.eof() && in.good()) {
        in.read(buff, 2048);
        last_read = in.gcount();
        out.write(buff, last_read);
        builder.write(reinterpret_cast<unsigned char*>(buff), last_read);
    }
    if (builder.finalize() != this->sha256) {
        throw record_exception("stored hash does not equal calculated hash");
    }
    return out;
}

} // namespace p2p
