//
// Created by Nick Grout on 2019-08-22.
//

#ifndef _RECORD_TABLE_H_
#define _RECORD_TABLE_H_

#include <iostream>
#include <memory>
#include "util/hash_table.h"
#include "util/hash.hpp"
#include "record.h"

#define RECORD_TABLE_SIZE 25

namespace p2p
{
using namespace util;

class RecordTable
{
    public:
        RecordTable();
        RecordTable(const RecordTable &rhs);
        RecordTable(RecordTable &&rhs) noexcept;
        ~RecordTable();
    protected:
    private:
        HashTable<Hash256, std::unique_ptr<Record>> record_table {RECORD_TABLE_SIZE};
};

} // namespace util
#endif //_RECORD_TABLE_H_
