// Nick Grout 6/26/2019

#ifndef P2P_NODE_
#define P2P_NODE_

#include <string>
#include <memory>
#include "util/hash.hpp"

namespace p2p
{
using namespace util;

class PeerNode
{
    public:
        PeerNode();
        PeerNode(const PeerNode &rhs);
        PeerNode(PeerNode &&rhs) noexcept;
        ~PeerNode();
        std::string retrieve(const Hash256& hash) const;
    protected:
    private:
};

} // namespace p2p
#endif // P2P_NODE_
