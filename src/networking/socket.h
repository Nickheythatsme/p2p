//
// Created by Nick Grout on 2019-09-03.
//

#ifndef _SOCKET_H_
#define _SOCKET_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <memory>
#include <iostream>

namespace p2p
{
namespace networking
{

using addrinfo_ptr = std::unique_ptr<struct addrinfo, decltype(&freeaddrinfo)>;

addrinfo_ptr init_addrinfo(const char* node, const char* service)
{
    int status;
    struct addrinfo hints;
    struct addrinfo* servinfo;

    memset(&hints, 0, sizeof hints); // make sure the struct is empty
    hints.ai_family = AF_UNSPEC;     // don't care IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
    hints.ai_flags = AI_PASSIVE;     // fill in my IP for me

    if ((status = getaddrinfo(node, service, &hints, &servinfo)) != 0) {
        std::cerr << "getaddrinfo error: " << gai_strerror(status) << '\n';
    }

    return addrinfo_ptr(servinfo, freeaddrinfo);
}


} // namespace networking
} // namespace p2p
#endif //_SOCKET_H_
