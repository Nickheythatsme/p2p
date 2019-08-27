//
// Created by Nick Grout on 2019-08-26.
//

#ifndef _RECORD_H_
#define _RECORD_H_

#include "util/hash.hpp"
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
        Record() = delete;
        explicit Record(Hash hash);
        Record(const Record &rhs) = default;
        Record(Record &&rhs) noexcept = default;
        virtual ~Record() = default;
        friend bool operator==(const Record& lhs, const Hash& rhs);
        friend bool operator==(const Record& lhs, const Record &rhs);
        virtual std::ostream& retrieve(std::ostream& in) = 0;
        virtual bool is_remote() const = 0;
    protected:
        Hash hash;
};

class LocalRecord: public Record
{
    public:
        LocalRecord() = delete;
        static LocalRecord CreateLocalRecord(const char* filename);
        explicit LocalRecord(Hash hash, const char *filename);
        LocalRecord(const LocalRecord &rhs) = default;
        LocalRecord(LocalRecord &&rhs) noexcept = default;
        std::ostream& retrieve(std::ostream& in) override;
        bool is_remote() const override;
    private:
        std::string filename;
};


} // namespace p2p
#endif //_RECORD_H_