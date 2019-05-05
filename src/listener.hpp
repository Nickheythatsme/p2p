//
// Created by Nick Grout on 2019-05-04.
//

#ifndef LISTENER_HPP
#define LISTENER_HPP

#include "util/logger.hpp"
#include "p2p_connection.hpp"
#include <unistd.h>
#include <future>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


namespace p2p {

class Listener
{
public:
    void start_listening(int port)
    {
        listener_thread = std::make_unique<std::thread>(&Listener::_start_listening, this, port);
    }
    ~Listener()
    {
        if (listener_thread && listener_thread->joinable())
        {
            logger.debug("Joining listener_thread");
            listener_thread->join();
        }
    }
protected:
    void _start_listening(int port)
    {
        logger.info("Starting listening");
        SockFd serverfd;
        ServAddr address;
        address.get()->sin_addr.s_addr = INADDR_ANY;
        address.get()->sin_port = port;

        logger.debug("Binding Socket to port");
        // Forcefully attaching socket to the port 8080
        if (bind(serverfd, (struct sockaddr *)address.get(),
                 sizeof(sockaddr_in))<0)
        {
            throw node_exception("Error binding");
        }
        logger.debug("Listening for new connections");
        if (listen(serverfd, 3) < 0)
        {
            perror("listen");
            return;
        }
        auto sockaddr_in_size = sizeof(struct sockaddr_in);
        logger.debug("Accepting new connections");
        while (running)
        {
            SockFd new_socket(accept(serverfd, (struct sockaddr *)address.get(), (socklen_t*)&sockaddr_in_size));
            if (new_socket < 0)
            {
                perror("accept");
                return;
            }
            logger.debug("Accepted new connection");
            std::thread(&Listener::read_respond, this, std::move(new_socket)).detach();
        }
    }
    bool read_respond(SockFd&& fd)
    {
        // Reading
        char buffer[1024];
        read(fd.get(), buffer, 1024);
        logger.debug(std::string("Request: ") + buffer);
        send(fd.get(), "pong", strlen("pong"), 0);
        logger.debug("Response sent");
        return true;
    }
private:
    static Logger logger;
    std::unique_ptr<std::thread> listener_thread {nullptr};
    std::atomic<bool> running {true};
};

Logger Listener::logger("Listener");


} // namespace p2p
#endif //LISTENER_HPP
