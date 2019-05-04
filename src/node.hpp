//
// Created by Nick Grout on 2019-04-06.
//

#include <vector>
#include <memory>
#include <exception>
#include <string>
#include "util/logger.hpp"

// For socket info
#include <sys/types.h>
#include <sys/socket.h>
#include <cerrno>
#include <arpa/inet.h> // For inet_pton

/*
int connect(int sockfd, const struct sockaddr *addr,
    socklen_t addrlen);
*/

#ifndef P2P_NODE_H
#define P2P_NODE_H


using std::vector;
namespace p2p {

// Classes used for exception handling
class node_exception: public std::exception
{
    public:
        node_exception() noexcept
            :
            msg(nullptr) {}
        explicit node_exception(const char *msg) noexcept
            :
            msg(msg) {}
        virtual const char *
        what() const noexcept override {
          if (msg)
            return msg;
          else
            return "Unknown node exception";
        }
    private:
        const char *msg;
};

class invalid_ip_exception: public node_exception 
{
    public:
        const char *
        what() const noexcept override {
          return "Invalid IP address or domain address";
        }
};

class socket_exception: public node_exception
{
    public:
        socket_exception() noexcept {
          error_at_init = errno;
        }
        const char *
        what() const noexcept override {
          switch (error_at_init) {
            case EACCES:return "Error creating socket: permission denied";
            case ENFILE:return "Error creating socket: The system file table is full.";
            case ENOBUFS:return "Error creating socket: Insufficient buffer space";
            case ENOMEM:return "Error creating socket: Insufficient memory to complete operation";
            default:return "Error creating socket: Unknown error.";

          }
        }
private:
        int error_at_init;
};

class Node
{
    public:
        Node(const char *address, unsigned short port)
            :
            addr(),
            last_contact(0) {
          init_sockaddr_in(address, port);
        }
        // Ping another node
        bool
        ping() {
          int sockfd = socket(AF_INET, SOCK_STREAM, 0);
          if (sockfd < 0) {
            throw socket_exception();
          }
          logger.info("connecting to server");
          if (connect(sockfd, (struct sockaddr *) &(*addr), sizeof(sockaddr_in)) < 0) {
            throw socket_exception();
          }
          // TODO add logging & time stats
          logger.info("Connection successful, attempting to send message.");
          send(sockfd, "ping!", strlen("ping!"), 0);
          logger.info("Ping sent");
          char buffer[1024];
          ssize_t valread = read(sockfd, buffer, 1024);
          logger.info(std::string("Response buffer: ") + std::string(buffer));
          return true;
        }
        // Single threaded. Listen for & respond to pings.
        void
        listen() {
        }
    protected:
        void
        init_sockaddr_in(const char *address, unsigned short port) {
          auto *addr_new = new sockaddr_in;
          // set all to '0', we use hexcode to be explicit, as '0' sometimes looks like 0
          memset(addr_new, 0x30, sizeof(sockaddr_in));

          addr_new->sin_family = AF_INET;
          addr_new->sin_port = port;
          if (inet_pton(AF_INET, address, &addr_new->sin_addr) <= 0) {
            throw invalid_ip_exception();
          }
          this->addr.reset(addr_new);
        }
        std::shared_ptr<struct sockaddr_in> addr;
        time_t last_contact;
    private:
        static Logger logger;
};

Logger Node::logger("Node");

} // namespace p2p
#endif //P2P_NODE_H
