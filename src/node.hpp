//
// Created by Nick Grout on 2019-04-06.
//

#ifndef P2P_NODE_H
#define P2P_NODE_H


#include <memory>
#include <exception>
#include <string>
#include <chrono>
#include "util/logger.hpp"
#include "util/config.hpp"
#include "p2p_connection.hpp"

namespace p2p {

using Clock = std::chrono::system_clock;
using TimePoint = std::chrono::time_point<Clock>;

class Node: public P2pConnection
{
    public:
        Node(const char* ip, const char* port):
            P2pConnection(ip, port, false)
        {
        }
        Node(Node&& rhs) noexcept :
            P2pConnection(std::move(rhs)),
            last_contact(rhs.last_contact)
        {
            rhs.last_contact = TimePoint::min();
        }
        // Ping another node
        bool ping()
        {
            if (!connected)
                _connect();
            logger.debug("Connected to node");
            if ( send(sockfd, "ping!", strlen("ping!"), MSG_NOSIGNAL) < 0) {
                logger.info(std::string("send to another node failed: ") + connection_exception().what());
                return false;
            }
            char buffer[1024];
            ssize_t valread = recv(sockfd, buffer, 1024, 0);
            buffer[valread] = '\0';
            logger.info("Response buffer: ", buffer);
            last_contact = std::chrono::system_clock::now();
            return true;
        }
        bool send_file(const char* contents, size_t len)
        {
            if (!connected) _connect();
            if (send(sockfd, contents, len, MSG_NOSIGNAL) < 0) {
                logger.info("send to another node failed: ", connection_exception().what());
                return false;
            }
            char buffer[1024];
            ssize_t valread = recv(sockfd, buffer, 1024, 0);
            buffer[valread] = '\0';
            logger.info("Response buffer: ", buffer);
            last_contact = std::chrono::system_clock::now();
            return true;
        }
        // Number of seconds since last contact
        long since_last_contact() const
        {
            auto now = Clock::now();
            auto diff = std::chrono::duration_cast<std::chrono::seconds>(now - last_contact);
            return diff.count();
        }
    protected:
        void _connect()
        {
            if (connect(sockfd, addr->ai_addr, addr->ai_addrlen) < 0) {
                logger.debug("Could not connect: ", connection_exception().what());
                return;
            }
            connected = true;
        }
        TimePoint last_contact {TimePoint::min()};
        bool connected {false};
    private:
        static Logger logger;
};

Logger Node::logger("Node");

} // namespace p2p
#endif //P2P_NODE_H
