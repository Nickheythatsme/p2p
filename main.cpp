#include <iostream>
#include "src/util/uuid.h"
#include <string>

using namespace p2p::util;
using std::cout;
using std::endl;

int main(int argc, char **argv)
{
    std::string test_uuid = "4c0272e2-5901-43b9-be4b-73b219abaf2f";
    auto u = UUID::parse(test_uuid.c_str());
    auto u_string = u.to_string();
    cout << "test uuid:   " << test_uuid << endl
         << "parsed uuid: " << u << endl;
    return 0;
}
