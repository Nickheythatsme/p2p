// Nick Grout 6/26/2019

#ifndef P2P_NODE_
#define P2P_NODE_

#include <string>
#include <memory>
#include <vector>
#include "record.h"
#include "util/hash.hpp"

namespace p2p
{
using namespace util;

class Node
{
    public:
        Node();
        Node(const Node &rhs);
        Node(Node &&rhs) noexcept;
        ~Node();
    protected:
    private:
        std::vector<Record> records;
};

} // namespace p2p
#endif // P2P_NODE_
