//
// Created by Nick Grout on 2019-08-27.
//

#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include "gtest/gtest.h"
#include "util/logging.h"

using std::cout;
using std::cerr;
using std::endl;

using namespace p2p::util;

TEST(LogWriter, smoke)
{
    LogWriter log_writer;
    log_writer.add_logs("testing");
    log_writer.add_logs("testing");
    log_writer.add_logs("testing");
    log_writer.add_logs("testing");
    std::this_thread::sleep_for(std::chrono::seconds(3));
    log_writer.add_logs("testing");
    log_writer.add_logs("testing");
    log_writer.add_logs("testing");
    log_writer.add_logs("testing");
}

