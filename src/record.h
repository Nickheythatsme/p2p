//
// Created by Nick Grout on 2019-08-26.
//

#ifndef _RECORD_H_
#define _RECORD_H_

#include "util/hash.hpp"

namespace p2p
{
using namespace util;

class Record
{
    public:
        Record() = default;
        Record(const Record &rhs) = default;
        Record(Record &&rhs) noexcept = default;
        virtual ~Record() = default;
        virtual std::ostream& retrieve(std::ostream& in, const Hash &hash) = 0;
        virtual bool is_remote() const = 0;
};

class LocalRecord : public Record
{
    public:
        LocalRecord();
        LocalRecord(const LocalRecord &rhs);
        LocalRecord(LocalRecord &&rhs) noexcept;
        ~LocalRecord() override;
        std::ostream& retrieve(std::ostream& in, const Hash &hash) override;
        bool is_remote() const override;
};

} // namespace p2p
#endif //_RECORD_H_
