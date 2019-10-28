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

} // namespace p2p
