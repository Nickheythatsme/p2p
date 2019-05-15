#include <memory>

//
// Created by Nick Grout on 2019-04-06.
//

#include <vector>
#include <memory>
#include <exception>
#include <cstring>

// For socket info
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <cerrno>
#include <arpa/inet.h> // For inet_pton
#include <netdb.h>


#ifndef P2P_PDP_CONNECTION_H
#define P2P_PDP_CONNECTION_H

namespace p2p {

// Classes used for exception handling
class connection_exception: public std::exception
{
    public:
        connection_exception() noexcept:
            msg(nullptr) {}
        explicit connection_exception(const char *msg) noexcept:
            msg(msg) {}
        virtual const char* what() const noexcept override
        {
            if (msg)
            {
                return msg;
            }
            return "Unknown node exception";
        }
    private:
        const char *msg;
};

class socket_exception: public connection_exception
{
    public:
        socket_exception() noexcept:
            error_at_init(errno)
        { }
        const char* what() const noexcept override {
            switch (error_at_init) {
                case EACCES: return "Error creating socket: permission denied";
                case ENFILE: return "Error creating socket: The system file table is full.";
                case ENOBUFS:return "Error creating socket: Insufficient buffer space";
                case ENOMEM: return "Error creating socket: Insufficient memory to complete operation";
                case ECONNREFUSED: return "Error connecting: Connection refused by server";
                case ENETUNREACH: return "Error connecting: Network unreachable";
                default:     return "Unknown socket error";

            }
        }
    private:
        int error_at_init;
};

class P2pConnection
{
    public:
        explicit P2pConnection(const char* address, const char* port, bool force_ipv6):
            sockfd(-1)
        {
            struct addrinfo hint{}, *temp_res = nullptr;
            int ret;

            hint.ai_family = (force_ipv6) ? AF_INET6 : AF_UNSPEC;
            hint.ai_socktype = SOCK_STREAM;
            hint.ai_flags = AI_NUMERICHOST | AI_NUMERICSERV | AI_PASSIVE;
            hint.ai_protocol = 0;
            if ( (ret = getaddrinfo(address, port, &hint, &temp_res)) )
            {
                throw connection_exception(gai_strerror(ret));
            }
            addr.reset(temp_res);

            init_socket();
        }
        P2pConnection(P2pConnection&& rhs) noexcept :
            sockfd(rhs.sockfd),
            addr(std::move(rhs.addr))
        {
            rhs.addr.reset();
            rhs.sockfd = 0;
        }
        virtual ~P2pConnection()
        {
            close(sockfd);
        }
    protected:
        int sockfd;
        std::unique_ptr<struct addrinfo, decltype(&freeaddrinfo)> addr {nullptr, &freeaddrinfo};
    private:
        void init_socket()
        {
            sockfd = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
            if (sockfd < 0)
            {
                perror("socket");
                throw socket_exception();
            }
        }
};

} // namespace p2p
#endif // P2P_PDP_CONNECTION_H


