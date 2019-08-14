//
// Created by Nick Grout on 2019-08-13.
//

#ifndef FILE_HPP
#define FILE_HPP

#include <fstream>
#include <string>
#include "./hash.hpp"

namespace p2p {

class File {
    public:
        File() = default;
        File(const char* fname);
        ~File() = default;
    protected:
        std::string fname;
        Hash256 hash256;
    private:
};

} // namespace p2p
#endif // FILE_HPP

