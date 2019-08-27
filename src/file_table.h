//
// Created by Nick Grout on 2019-08-22.
//

#ifndef _FILE_TABLE_H_
#define _FILE_TABLE_H_

#include "util/hash_table.h"
#include "util/hash.hpp"

namespace p2p
{
using namespace util;

class FileTable
{
    public:
        FileTable();
        FileTable(const FileTable &rhs);
        FileTable(FileTable &&rhs);
        ~FileTable();
    protected:
    private:
};

} // namespace util
#endif //_FILE_TABLE_H_
