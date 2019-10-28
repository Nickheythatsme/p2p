//
// Created by Nick Grout on 2019-08-26.
//

#ifndef _RECORD_H_
#define _RECORD_H_

#include "util/hash.hpp"
#include "networking/serialize.h"
#include "util/uuid.h"
#include <fstream>
#include <exception>

namespace p2p
{
using namespace util;

class record_exception : public std::exception
{
    public:
        explicit record_exception(const char *what_arg) :
        std::exception(),
        what_arg(what_arg)
        {
        }
        const char *what() const noexcept override
        {
            return what_arg;
        }
        record_exception &operator=(const exception &other) noexcept
        {
            what_arg = other.what();
            return *this;
        }
    private:
        const char *what_arg;
};

class Record
{
    public:
        Record() = default;
        explicit Record(UUID uuid);
        Record(const Record &rhs) = default;
        Record(Record &&rhs) noexcept = default;
        virtual ~Record() = default;
        friend bool operator==(const Record& lhs, const UUID& rhs);
        friend bool operator==(const Record& lhs, const Record &rhs);
        virtual std::ostream& retrieve(std::ostream& in) = 0;
        const UUID& get_uuid() const;
    protected:
        UUID uuid;
        std::string filename;
        uint64_t record_length;
        // Hash256 sha256; // to verify the integrity of the record
};

} // namespace p2p
#endif //_RECORD_H_
