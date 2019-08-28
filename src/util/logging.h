//
// Created by Nick Grout on 2019-08-27.
//

#ifndef _LOGGING_HPP_
#define _LOGGING_HPP_

#include <thread>
#include <chrono>
#include <iostream>
#include <mutex>
#include <memory>
#include <atomic>
#include <vector>
#include <sstream>
#include <condition_variable>
#include <cstdio>

namespace p2p {
namespace util {

class LogWriter
{
    public:
        LogWriter();
        LogWriter(const LogWriter &rhs) = delete;
        LogWriter(LogWriter &&rhs) noexcept = delete;
        ~LogWriter();
        template<typename T, typename ...Args>
        LogWriter& write(const T& t, Args ...args);
        template<typename T>
        LogWriter& write(const T& t);
    protected:
        void output_logs();
    private:
        template<typename T>
        const T& _write(const T& t);
        std::condition_variable logs_ready;
        std::thread logging_thread;
        std::stringstream logs;
        std::mutex logs_lock;
        std::atomic_bool running {false};
        std::atomic_bool logs_to_write {false};
};

class Logger
{
    public:
        Logger();
        Logger(std::string tag);
        Logger(const Logger& rhs) = default;
        Logger(Logger &&rhs) noexcept;
        ~Logger();
        template<typename ...T>
        Logger& write(T ...t);
    protected:
    private:
        std::string tag;
        static void init_log_writer();
        static std::mutex log_writer_lock;
        static std::unique_ptr<LogWriter> log_writer;
};

} // namespace util
} // namespace p2p
#include "logging.cpp"
#endif //_LOGGING_HPP_
