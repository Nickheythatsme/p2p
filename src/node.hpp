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
            logger.debug("Created socket");
            // Connect
            logger.debug("Connecting to node");
            if (connect(fd.get(), (struct sockaddr *)addr.get(), sizeof(sockaddr_in)) < 0) {
                logger.debug(std::string("Could not connect: ") + connection_exception().what());
                return false;
            }
            logger.debug("Connected to node");
            // TODO error handling
            send(fd.get(), "ping!", strlen("ping!"), 0);
            char buffer[1024];
            ssize_t valread = read(fd.get(), buffer, 1024);
            buffer[valread] = '\0';
            logger.info(std::string("Response buffer: ") + std::string(buffer));
            return true;
        }
    protected:
        time_t last_contact;
        ServAddr addr;
    private:
        static Logger logger;
};

Logger Node::logger("Node");

} // namespace p2p
#endif //P2P_NODE_H
