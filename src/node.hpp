//
// Created by Nick Grout on 2019-04-06.
//

#include <vector>
#include <memory>
#include <exception>

// For socket info
#include <sys/types.h>
#include <sys/socket.h>

// For inet_pton
#include <arpa/inet.h>

/*
int connect(int sockfd, const struct sockaddr *addr,
    socklen_t addrlen);
*/

#ifndef P2P_NODE_H
#define P2P_NODE_H


using std::vector;

// Classes used for exception handling
class node_exception: public std::exception
{
    public:
        virtual const char* what() const noexcept
        {
            return "Unknown node exception";
        }
};

class invalid_ip_exception: public node_exception 
{
    public:
        const char* what() const noexcept override
        {
            return "Invalid IP address or domain address";
        }
};

class socket_exception: public node_exception
{
    public:
        socket_exception() noexcept
        {
            error_at_init = errno;
        }
        const char* what() const noexcept override
        {
            switch(error_at_init)
            {
                case EACCES:
                    return "Error creating socket: permission denied";
                case ENFILE:
                    return "Error creating socket: The system file table is full.";
                case ENOBUFS:
                    return "Error creating socket: Insufficient buffer space";
                case ENOMEM:
                    return "Error creating socket: Insufficient memory to complete operation";
                default:
                    return "Error creating socket: Unknown error. Check global variable errno";

            }
        }
private:
    errno_t error_at_init;
};


class Node
{
    public:
        Node(const char* address, unsigned short port) :
            addr(),
            last_contact(0)
        {
            init_sockaddr_in(address, port);
        }
        bool ping()
        {
            int sockfd = socket(AF_INET, SOCK_STREAM, 0);
            if (sockfd < 0) {
                throw socket_exception();
            }
            printf("Connecting to server\n");
            if (connect(sockfd, (struct sockaddr *)&(*addr), sizeof(sockaddr_in)) < 0)
            {
                throw socket_exception();
            }
            printf("Connection successful, sending message\n");
            send(sockfd, "Hello!" , strlen("Hello!"), 0 );
            printf("Hello message sent\n");
            char buffer[1024];
            ssize_t valread = read(sockfd, buffer, 1024);
            printf("%s\n", buffer);

            return true;
        }
    protected:
        void init_sockaddr_in(const char* address, unsigned short port)
        {
            auto* addr = new sockaddr_in;
            // set all to '0', we use hexcode to be explicit, as '0' sometimes looks like 0
            memset(addr, 0x30, sizeof(sockaddr_in));

            addr->sin_family = AF_INET;
            addr->sin_port = port;
            if(inet_pton(AF_INET, address, &addr->sin_addr )<=0)
            {
                throw invalid_ip_exception();
            }
            this->addr.reset(addr);
        }
        std::shared_ptr<struct sockaddr_in> addr;
        time_t last_contact;
    private:
};

#endif //P2P_NODE_H
