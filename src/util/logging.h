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
#include <condition_variable>

namespace p2p {
namespace util {

class LogWriter
{
    public:
        LogWriter();
        LogWriter(const LogWriter &rhs) = delete;
        LogWriter(LogWriter &&rhs) noexcept = delete;
        ~LogWriter();
        void add_logs(std::string logs);
    protected:
        void output_logs(std::condition_variable& ready);
    private:
        std::thread logging_thread;
        std::vector<std::string> logs;
        std::mutex logs_lock;
        std::atomic_bool running {false};
};

} // namespace util
} // namespace p2p
#endif //_LOGGING_HPP_
