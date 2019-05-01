//
// Created by Nick Grout on 2019-04-30.
//

#ifndef P2P_RUNNABLE_HPP
#define P2P_RUNNABLE_HPP

namespace p2p {

class Runnable
{
public:
    virtual void run(int x) = 0;
    virtual ~Runnable() = 0;
};

} // namespace p2p

#endif //P2P_RUNNABLE_HPP
