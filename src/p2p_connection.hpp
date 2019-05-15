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

// INIT a manage a socket. Each socket is treated just like a file, so 
// it must be closed. 
// This class makes sure the socket is closed once it's out of scope
class SockFd
{
    public:
        SockFd():
            _sockfd(init_socket())
        { }
        SockFd(const SockFd& rhs) = delete;
        SockFd(SockFd &&rhs) noexcept:
            _sockfd(rhs._sockfd)
        {
            rhs._sockfd = -1;
        }
        explicit SockFd(int sockfd) noexcept:
            _sockfd(sockfd)
        { }
        operator int() const
        {
            return _sockfd;
        }
        SockFd& operator=(int sockfd)
        {
            close(_sockfd);
            _sockfd = sockfd;
            return *this;
        }
        SockFd& operator=(SockFd &&rhs) noexcept
        {
            close(_sockfd);
            _sockfd = rhs._sockfd;
            rhs._sockfd = 0;
            return *this;
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

class client_connection 
{
    public:
        client_connection(const char* address, const char* port)
        {
            struct addrinfo hint, *temp_res = nullptr;
            int ret;

            memset(&hint, '\0', sizeof hint);

            hint.ai_family = PF_UNSPEC;
            hint.ai_flags = 0;
            hint.ai_protocol = 0;
            hint.ai_socktype = SOCK_STREAM;
            if ( (ret = getaddrinfo(address, port, &hint, &temp_res)) )
            {
                throw connection_exception(gai_strerror(ret));
            }
            addr.reset(temp_res);

            init_socket();
        }
        client_connection(client_connection&& rhs):
            sockfd(rhs.sockfd),
            addr(std::move(rhs.addr))
        {
            rhs.addr.reset();
            rhs.sockfd = 0;
        }
        virtual ~client_connection()
        {
            close(sockfd);
        }
    protected:
        int sockfd;
        std::unique_ptr<struct addrinfo> addr;
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


class ServAddr
{
    public:
        explicit ServAddr(const char* address, int port)
        {
            init_sockaddr_in();
            set_sin_addr(address, port);
        }
        ServAddr()
        {
            init_sockaddr_in();
        }
        struct sockaddr_in* get()
        {
            return addr.get();
        }
    protected:
        void init_sockaddr_in()
        {
            addr = std::make_unique<struct sockaddr_in>();
            // set all to '0', we use hexcode to be explicit, as '0' sometimes looks like 0
            memset(addr.get(), 0x30, sizeof(struct sockaddr_in));
            addr->sin_family = AF_INET;
        }
        void set_sin_addr(const char *address, unsigned short port)
        {
            if (!addr) return;
            addr->sin_port = port;
            if (inet_pton(AF_INET, address, &addr->sin_addr) == 0) {
                throw connection_exception("Invalid ip address");
            }
        }
    private:
        std::unique_ptr<struct sockaddr_in> addr {nullptr};
};


} // namespace p2p
#endif // P2P_PDP_CONNECTION_H


