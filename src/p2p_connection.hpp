//
// Created by Nick Grout on 2019-04-06.
//

#include <vector>
#include <memory>
#include <exception>
#include <string>

// For socket info
#include <sys/types.h>
#include <sys/socket.h>
#include <cerrno>
#include <arpa/inet.h> // For inet_pton

#ifndef P2P_PDP_CONNECTION_H
#define P2P_PDP_CONNECTION_H

namespace p2p {

// Classes used for exception handling
class node_exception: public std::exception
{
    public:
        node_exception() noexcept:
            msg(nullptr) {}
        explicit node_exception(const char *msg) noexcept:
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

class socket_exception: public node_exception
{
    public:
        socket_exception() noexcept:
            msg(nullptr),
            error_at_init(errno)
        { }
        socket_exception(const char* msg) noexcept:
            msg(msg),
            error_at_init(errno)
        { }
        const char* what() const noexcept override {
            if (msg)
            {
                return msg;
            }
            switch (error_at_init) {
                case EACCES: return "Error creating socket: permission denied";
                case ENFILE: return "Error creating socket: The system file table is full.";
                case ENOBUFS:return "Error creating socket: Insufficient buffer space";
                case ENOMEM: return "Error creating socket: Insufficient memory to complete operation";
                case ECONNREFUSED: return "Error connecting: Connection refuested by server";
                case ENETUNREACH: return "Error connecting: Network unreachable";
                default:     return "Unknown socket error";

            }
        }
    private:
        const char* msg;
        int error_at_init;
};

// INIT a manage a socket. Each socket is treated just like a file, so 
// it must be closed. 
// This class makes sure the socket is closed once it's out of scope
class SockFd
{
    public:
        SockFd():
            _sockfd(init_socket())
        {
        }
        SockFd(const SockFd& rhs) = delete;
        SockFd(SockFd &&rhs):
            _sockfd(rhs._sockfd),
            _closed(rhs._closed)

        {
            rhs._sockfd = -1;
            rhs._closed = true;
        }
        ~SockFd()
        {
            close(_sockfd);
        }
        int get() const
        {
            return _sockfd;
        }
    private:
        int _sockfd;
        bool _closed {false};
        static int init_socket()
        {
            int sockfd = socket(AF_INET, SOCK_STREAM, 0);
            if (sockfd < 0)
            {
                throw socket_exception();
            }
            return sockfd;
        }
};

class ServAddr
{
    public:
        ServAddr(const char* address, int port):
            addr(init_sockaddr_in(address, port))
        {
            if (addr.get() == nullptr)
            {
                throw node_exception("Invalid ip address");
            }
        }
        struct sockaddr_in* get()
        {
            return addr.get();
        }
    protected:
        static struct sockaddr_in* init_sockaddr_in(const char *address, unsigned short port)
        {
            auto *addr = new sockaddr_in;
            // set all to '0', we use hexcode to be explicit, as '0' sometimes looks like 0
            memset(addr, 0x30, sizeof(sockaddr_in));

            addr->sin_family = AF_INET;
            addr->sin_port = port;
            if (inet_pton(AF_INET, address, &addr->sin_addr) == 0) {
                return nullptr;
            }
            return addr;
        }
    private:
        std::unique_ptr<struct sockaddr_in> addr;
};


} // namespace p2p
#endif // P2P_PDP_CONNECTION_H


