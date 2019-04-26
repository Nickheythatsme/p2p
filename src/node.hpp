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
#define _OPEN_SYS_SOCK_IPV6
#include <arpa/inet.h>

/*
int connect(int sockfd, const struct sockaddr *addr,
    socklen_t addrlen);
*/

#ifndef P2P_NODE_H
#define P2P_NODE_H


using std::vector;

class node_exception: public std::exception
{
};

class invalid_ip_exception: public node_exception 
{
    public:
        const char* what() const noexcept override
        {
            return "Invalid IP address or domain address";
        }
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
            // TODO implement
            return true;
        }
    protected:
        void init_sockaddr_in(const char* address, unsigned short port)
        {
            sockaddr_in* addr = new sockaddr_in;
            memset(addr, '0', sizeof(sockaddr_in));

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
