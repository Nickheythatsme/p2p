//
// Created by Nick Grout on 2019-05-15.
//

#ifndef CONFIG_HPP
#define CONFIG_HPP

#if defined(__unix__) || defined(__APPLE__) || defined(__linux__)
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <cerrno>

#if defined(__APPLE__) || defined(__FreeBSD__)

#undef  MSG_NOSIGNAL
#define MSG_NOSIGNAL SO_NOSIGPIPE

#else
#endif
#else // Windows machine
// TODO add support for windows
#endif

#endif //CONFIG_HPP
