//
// Created by Nick Grout on 2019-08-26.
//

#include "record.h"

namespace p2p
{
using namespace util;

LocalRecord::LocalRecord()
{

}
LocalRecord::LocalRecord(const LocalRecord &rhs)
{

}
LocalRecord::LocalRecord(LocalRecord&& rhs) noexcept
{

}
LocalRecord::~LocalRecord()
{

}
std::ostream& LocalRecord::retrieve(std::ostream &in, const Hash &hash)
{
    return in;
}
bool LocalRecord::is_remote() const
{
    return false;
}

} // namespace p2p
