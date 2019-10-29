//
// Created by Nick Grout on 2019-08-26.
//

#include "node.h"

namespace p2p
{

Node::Node()
{

}

Node::Node(const Node &rhs):
    records(rhs.records)
{

}

Node::Node(Node &&rhs) noexcept :
    records(std::move(rhs.records))
{

}

Node::~Node()
{

}

}
