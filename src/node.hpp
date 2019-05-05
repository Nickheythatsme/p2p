//
// Created by Nick Grout on 2019-04-06.
//

#include <vector>
#include <memory>
#include <exception>
#include <string>
#include "util/logger.hpp"
#include "p2p_connection.hpp"

// For socket info
#include <sys/types.h>
#include <sys/socket.h>
#include <cerrno>
#include <arpa/inet.h> // For inet_pton

#ifndef P2P_NODE_H
#define P2P_NODE_H


namespace p2p {

class Node
{
    public:
        Node(const char* ip, unsigned short port):
            last_contact(0),
            addr(ip, port)
        {
        }
        // Ping another node
        bool ping()
        {
            SockFd fd;
            _connect(fd);
            // TODO error handling
            /*
            send(sockfd.get(), "ping!", strlen("ping!"), 0);
            logger.info("Ping sent");
            char buffer[1024];
            ssize_t valread = read(sockfd.get(), buffer, 1024);
            logger.info(std::string("Response buffer: ") + std::string(buffer));
            */
            return true;
        }
    protected:
        void _connect(SockFd& sockfd)
        {
            logger.debug("Connecting to node");
            if (connect(sockfd.get(), (struct sockaddr *)addr.get(), sizeof(sockaddr_in)) < 0) {
                socket_exception se;
                logger.warn(std::string("Error trying to connect to node:") + se.what());
                throw se;
            }
        }
        time_t last_contact;
        ServAddr addr;
    private:
        static Logger logger;
};

Logger Node::logger("Node");

} // namespace p2p
#endif //P2P_NODE_H
