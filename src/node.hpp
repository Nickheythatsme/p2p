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

class Node: public p2p::P2pConnection
{
    public:
        Node(const char* ip, const char* port):
            P2pConnection(ip, port),
            last_contact(0)
        {
        }
        Node(Node&& rhs) noexcept :
            P2pConnection(std::move(rhs)),
            last_contact(rhs.last_contact)
        {
            rhs.last_contact = 0;
        }
        // Ping another node
        bool ping()
        {
            if (connect(sockfd, addr->ai_addr, addr->ai_addrlen) < 0) {
                perror("connect");
                std::stringstream ss;
                ss << "Could not connect: " << connection_exception().what();
                logger.debug(ss.str());
                return false;
            }
            logger.debug("Connected to node");
            // TODO error handling
            send(sockfd, "ping!", strlen("ping!"), 0);
            char buffer[1024];
            ssize_t valread = read(sockfd, buffer, 1024);
            buffer[valread] = '\0';
            logger.info(std::string("Response buffer: ") + std::string(buffer));
            return true;
        }
    protected:
        time_t last_contact;
    private:
        static Logger logger;
};

Logger Node::logger("Node");

} // namespace p2p
#endif //P2P_NODE_H
