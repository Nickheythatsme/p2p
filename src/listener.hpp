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
#include <cstdlib>
#include <cstring>
#include <csignal>


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
        stop_listening();
    }
    void stop_listening()
    {
        if (listener_thread && listener_thread->joinable())
        {
            logger.debug("Stopping listener_thread");
            auto listener_thread_handle = listener_thread->native_handle();
            // TODO implement signal handler
            // pthread_kill(listener_thread_handle, SIGINT);
            pthread_cancel(listener_thread_handle);
            listener_thread->join();
            listener_thread.reset(nullptr);
        }
    }
protected:
    void _start_listening(int port)
    {
        SockFd serverfd;
        ServAddr address;
        address.get()->sin_addr.s_addr = INADDR_ANY;
        address.get()->sin_port = port;

        logger.debug("Binding Socket to port");
        // Forcefully attaching socket to the port
        if (bind(serverfd, (struct sockaddr *)address.get(), sizeof(sockaddr_in)) < 0)
        {
            perror("bind()");
            throw node_exception("Error binding");
        }
        if (listen(serverfd, 3) < 0)
        {
            perror("listen()");
            throw node_exception("Error listening");
        }
        logger.info(std::string("Listening for new connections on port: ") + std::to_string(port));
        _accept_connections(std::move(serverfd), std::move(address));
    }
    void _accept_connections(SockFd&& fd, ServAddr&& addr)
    {
        // Register interrupt handler
        logger.debug("waiting for new connections");
        auto sockaddr_in_size = sizeof(struct sockaddr_in);
        logger.debug("Accepting new connections");
        SockFd new_socket(accept(fd, (struct sockaddr *)addr.get(), (socklen_t*)&sockaddr_in_size));
        if (new_socket < 0)
        {
            perror("accept");
            throw node_exception("Error when accepting new connection");
        }
        logger.debug("Accepted new connection");
        std::thread(&Listener::read_respond, this, std::move(new_socket)).detach(); // TODO create threadpool
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
};

Logger Listener::logger("Listener");


} // namespace p2p
#endif //LISTENER_HPP
