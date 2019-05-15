#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp"
//
// Created by Nick Grout on 2019-04-09.
//

#include "gtest/gtest.h"
#include "../src/node.hpp"
#include "../src/util/config.hpp"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <sys/sysctl.h>

using namespace p2p;
#define PAUSE(T) (std::this_thread::sleep_for(std::chrono::milliseconds(T)))

TEST(Node, SmokeTest) {
    Node *n = new Node("0.0.0.0", "8080");
    delete n;
}

TEST(Node, InvalidAddress) {
    Logger logger("Main");
    Logger::use_console();
    std::vector<std::string> good_addresses {
        "19.117.63.126",
        "255.255.253.0",
        "FE80:0000:0000:0000:0202:B3FF:FE1E:8329",
        "FE80::0202:B3FF:FE1E:8329"
    };
    std::vector<std::string> bad_addresses {
        "ffff.8.8.8.8",
        "FZ180:0000:0000:0000:0202:B3FF:FE1E:8329"
    };

    // Should not fail
    for (const std::string addr : good_addresses)
    {
        try {
            Node n(addr.c_str(), "8080");
        }
        catch(const connection_exception &e) {
            logger.info(std::string("Exception thrown: ") + e.what());
            PAUSE(100);
            FAIL() << "Exception was thrown when it was suppossed to";
        }
    }

    // Should fail
    for (const std::string addr : bad_addresses)
    {
        try {
            Node n(addr.c_str(), "8080");
            PAUSE(100);
            FAIL() << "Exception was not thrown for invalid IP address";
        }
        catch(const connection_exception &e) {
            std::stringstream ss;
            ss << "Exception thrown: " << e.what() << " Address: " << addr;
            logger.info(ss.str());
            PAUSE(100);
        }
    }
    PAUSE(100);
}

int get_number_of_sockets()
{
    std::stringstream ss;
    ss << "ls -l /proc/" << getpid() << "/fd | grep \"socket\" > socket_count.out";
    system(ss.str().c_str());

    std::ifstream fin("socket_count.out");
    int count=0;
    fin.ignore(1024,'\n');
    while (fin.good())
    {
        ++count;
        fin.ignore(1024,'\n');
    }
    remove("socket_count.out");
    return count;
}


// Skip test if not on linux, since proc is only available there
#ifdef __linux__
TEST(Node, SocketLeakage)
{
    Logger logger("Main");
    Logger::use_console();

    {
        std::stringstream ss;
        ss << "Using " << get_number_of_sockets() << " sockets";
        logger.info(ss.str());
    }

    {
        std::vector<Node> nodes;

        for (int i=0; i<100; ++i)
        {
            nodes.emplace_back("127.0.0.1", "8080");
        }
        PAUSE(1000);
        std::stringstream ss;
        ss << "Using " << get_number_of_sockets() << " sockets";
        logger.info(ss.str());
    }

    std::stringstream ss;
    ss << "Using " << get_number_of_sockets() << " sockets";
    logger.info(ss.str());
    PAUSE(100);
}
TEST(Node, SocketAllocError)
{
    Logger logger("MAIN");
    Logger::use_console();
    std::vector<Node> nodes;

    try {
        for (unsigned i = 0; i < 0xFFFF; ++i)
        {
            nodes.emplace_back(Node("127.0.0.1", "8080"));
            i++;
            if (i % 1000 == 0)
            {
                logger.info(std::to_string(i) + std::string(" sockets created"));
            }
        }
    } catch(const socket_exception& e)
    {
        logger.info(std::string("Caught socket exception") + e.what());
        SUCCEED() << "Successfully threw exception";
        return;
    } catch(const std::exception &e)
    {
        FAIL() << "Wrong exception was thrown";
    }
}
#endif

#pragma clang diagnostic pop
