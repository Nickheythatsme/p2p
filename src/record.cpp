#include <utility>

//
// Created by Nick Grout on 2019-08-26.
//

#include "record.h"
#include <iostream>

namespace p2p {
using namespace util;

Record::Record(Hash256 hash):
    hash(std::move(hash))
{ }

bool operator==(const Record& lhs, const Hash256& rhs)
{
    return lhs.hash == rhs;
}

bool operator==(const Record& lhs, const Record &rhs)
{
    return lhs.hash == rhs.hash;
}

LocalRecord::LocalRecord(Hash256 hash, const char *filename):
    Record(std::move(hash)),
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
    return LocalRecord(builder.finalize(), filename);
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
    if (builder.finalize() != this->hash) {
        throw record_exception("stored hash does not equal calculated hash");
    }
    return out;
}

} // namespace p2p
