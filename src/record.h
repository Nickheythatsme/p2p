//
// Created by Nick Grout on 2019-08-26.
//

#ifndef _RECORD_H_
#define _RECORD_H_

#include "util/hash.hpp"
#include "util/uuid.h"
#include "networking/serialize.h"
#include <fstream>
#include <exception>

namespace p2p
{
using namespace util;
using namespace networking;

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

class Record: public Serializable
{
    public:
        static Record build(std::string record_contents);
        Record() = default;
        explicit Record(UUID uuid, Hash256 sha256, std::string record_contents);
        Record(const Record &rhs) = default;
        Record(Record &&rhs) noexcept = default;
        virtual ~Record() = default;
        friend bool operator==(const Record& lhs, const UUID& rhs);
        friend bool operator==(const Record& lhs, const Record &rhs);
        const UUID& get_uuid() const;
        const Hash256& get_hash256() const;
        const std::string& get_record_contents() const;
        std::ostream& serialize(std::ostream& out) const;
        std::istream& unserialize(std::istream& in);
    protected:
        UUID uuid; // key
        Hash256 sha256; // value
        std::string record_contents;
};

} // namespace p2p
#endif //_RECORD_H_
