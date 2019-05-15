//
// Created by Nick Grout on 2019-05-04.
//

#ifndef LISTENER_HPP
#define LISTENER_HPP

#include "util/logger.hpp"
#include "util/config.hpp"
#include "p2p_connection.hpp"
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <cstdlib>
#include <cstring>
#include <csignal>

// how long the connection queue can be before we send conn refused
#define BACKLOG 10 

#if defined(__apple__)
#define MSG_NOSIGPIPE SO_NOSIGPIPE
#define MSG_REUSEADDR SO_REUSEADDR
#elif defined(__linux__)
// nothing
#else
// TODO add windows support for broken pipe interrupt 
#define NOSIGPIPE 0
#endif

namespace p2p {

class Listener: public P2pConnection
{
public:
    Listener() = delete;
    Listener(const char* cport, bool force_ipv6) noexcept:
        port(cport),
        P2pConnection(nullptr, cport, force_ipv6)
    {
    }
    Listener(const char* cport) noexcept:
        port(cport),
        P2pConnection(nullptr, cport, false)
    {
    }
    void start_listening()
    {
        int opt = 1;
        if (setsockopt(sockfd, SOL_SOCKET, MSG_REUSEADDR,
                       &opt, sizeof(opt)))
        {
            throw connection_exception();
        }

        logger.debug("Binding Socket to port");
        if (bind(sockfd, addr->ai_addr, addr->ai_addrlen) < 0)
        {
            throw connection_exception("Error binding");
        }
        if (listen(sockfd, BACKLOG) < 0)
        {
            perror("listen");
            throw connection_exception("Error listening");
        }
        // get host name and port message
        char chostname[256];
        gethostname(chostname, 256);
        std::stringstream ss;
        ss << "Listening on port: " << port << " Hostname: " << chostname;
        logger.info(ss.str());
        listener_thread = std::make_unique<std::thread>(&Listener::_accept_connections, this);
    }
    bool using_ipv6() const
    {
        return addr->ai_family == AF_INET6;
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
        send(new_socket, "pong", strlen("pong"), MSG_NOSIGPIPE);
        logger.debug("Response sent");
        close(new_socket);
        return true;
    }
private:
    static Logger logger;
    std::unique_ptr<std::thread> listener_thread {nullptr};
    const std::string port;
};

Logger Listener::logger("Listener");

} // namespace p2p
#endif //LISTENER_HPP
