// Nick Grout 6/26/2019

#ifndef P2P_ROUTING_
#define P2P_ROUTING_

#include <string>
#include <memory>

namespace p2p {

class File {
  public:
    File() = default;
    File(const File &rhs) :
        uuid(rhs.uuid) {}
  protected:
  private:
};

} // namespace p2p
#endif // P2P_ROUTING_