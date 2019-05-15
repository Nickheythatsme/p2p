//
// Created by Nick Grout on 2019-04-06.
//

#include <vector>
#include <memory>
#include <exception>
#include <string>
#include "util/logger.hpp"
#include "util/config.hpp"
#include "p2p_connection.hpp"

// For socket info
#include <sys/types.h>
#include <sys/socket.h>
#include <cerrno>
#include <arpa/inet.h> // For inet_pton

#ifndef P2P_NODE_H
#define P2P_NODE_H

#if defined __apple__
#define NOSIGPIPE SO_NOSIGPIPE
#elif defined __linux__
#define NOSIGPIPE MSG_NOSIGNAL
#endif


namespace p2p {

class Node: public P2pConnection
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
            if (!connected)
                _connect();
            logger.debug("Connected to node");
            // TODO error handling
            if ( send(sockfd, "ping!", strlen("ping!"), NOSIGPIPE) < 0) {
                perror("send");
                logger.info("send to another node failed");
                return false;
            }
            char buffer[1024];
            ssize_t valread = recv(sockfd, buffer, 1024, 0);
            buffer[valread] = '\0';
            logger.info(std::string("Response buffer: ") + std::string(buffer));
            return true;
        }
        bool send_file(const char* filename)
        {
            std::ifstream fin(filename, std::ios::binary | std::ios::ate);
            if (!fin) {
                logger.info(std::string("invalid filename: ") + filename);
                return false;
            }
            auto flen = fin.tellg();
            char* contents = (char*) malloc(sizeof(char) * flen);
            if (contents == nullptr) {
                logger.info("Not enough memory");
                return false;
            }
            fin.seekg(std::ios_base::end);
            fin.read(contents, flen);

            if (!connected) _connect();
            if (send(sockfd, contents, flen, 0) < 0) {
                perror("send");
                logger.info("send to another node failed");
                return false;
            }
            char buffer[1024];
            ssize_t valread = recv(sockfd, buffer, 1024, 0);
            buffer[valread] = '\0';
            logger.info(std::string("Response buffer: ") + std::string(buffer));
            return true;
        }
    protected:
        void _connect()
        {
            if (connect(sockfd, addr->ai_addr, addr->ai_addrlen) < 0) {
                perror("connect");
                std::stringstream ss;
                ss << "Could not connect: " << socket_exception().what();
                logger.debug(ss.str());
                return;
            }
            connected = true;
        }
        time_t last_contact;
        bool connected {false};
    private:
        static Logger logger;
};

Logger Node::logger("Node");

} // namespace p2p
#endif //P2P_NODE_H
