//
// Created by Nick Grout on 2019-08-13.
//

#ifndef DATA_OBJECT_HPP
#define DATA_OBJECT_HPP

#include <fstream>
#include <string>
#include "hash.hpp"
#include "uuid.h"

namespace p2p {

class DataObject {
public:
    DataObject() = default;
    DataObject(HashObject hash, UUID uuid, std::string filepath);
    DataObject(const DataObject& data_object) = default;
    DataObject(DataObject&& data_object);
    ~DataObject() = default;
protected:
    HashObject hash;
    UUID uuid;
    std::string filepath;
private:
};

} // namespace p2p
#endif // DATA_OBJECT_HPP

