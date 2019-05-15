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

class Listener: public P2pConnection
{
public:
    Listener() = delete;
    Listener(const char* cport) noexcept:
        port(cport),
        P2pConnection(nullptr, cport)
    {

    }
    void start_listening()
    {
        int opt = 1;
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR,
                       &opt, sizeof(opt)))
        {
            perror("setsockopt");
            throw connection_exception("Error in setsockopt");
        }

        logger.debug("Binding Socket to port");
        if (bind(sockfd, addr->ai_addr, addr->ai_addrlen) < 0)
        {
            perror("bind");
            throw connection_exception("Error binding");
        }
        if (listen(sockfd, 3) < 0)
        {
            perror("listen");
            throw connection_exception("Error listening");
        }
        logger.info(std::string("Listening for new connections on port: ") + port);
        // _accept_connections();
        listener_thread = std::make_unique<std::thread>(&Listener::_accept_connections, this);
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
    void _accept_connections()
    {
        // TODO Register interrupt handler
        logger.debug("Waiting for new connections");
        for (;;)
        {
            int new_socket = accept(sockfd, addr->ai_addr, &addr->ai_addrlen);
            if (new_socket < 0)
            {
                perror("accept");
                throw connection_exception("Error when accepting new connection");
            }
            logger.debug("Accepted new connection");
            std::thread(&Listener::read_respond, this, new_socket).detach(); // TODO create threadpool
        }
    }
    // TODO do routing here. Is it a ping? A file? A routing table?
    bool read_respond(int new_socket)
    {
        // Reading
        std::stringstream ss;
        int bytes_read = 0;
        do {
            char buffer[1024];
            if ( (bytes_read = recv(new_socket, buffer, 1024, MSG_DONTWAIT)) < 0) {
                if (errno != EAGAIN && errno != EWOULDBLOCK) {
                    perror("recv");
                    return false;
                }
                bytes_read = 0;
            }
            buffer[bytes_read] = '\0';
            ss << buffer;
        } while (bytes_read > 0);
        logger.debug(std::string("Request: ") + ss.str());
        send(new_socket, "pong", strlen("pong"), 0);
        logger.debug("Response sent");
        close(new_socket);
        return true;
    }
private:
    static Logger logger;
    std::unique_ptr<std::thread> listener_thread {nullptr};
    std::string port;
};

Logger Listener::logger("Listener");

} // namespace p2p
#endif //LISTENER_HPP
