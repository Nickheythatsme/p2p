// 
// Nicholas Grout 4/30
//

#ifndef P2P_LOGGER_
#define P2P_LOGGER_
#include <string>
#include <memory>
#include <mutex>
#include <iostream>
#include <thread>
#include <atomic>

#include "runnable.hpp"

namespace p2p {


class Logger
{
    public:
        static void run()
        {
            while(running)
            {
                usleep(100);
                if (!log_queue.empty())
                {
                    std::lock_guard<std::mutex> guard(log_queue_lock);
                    std::cout << "Printing " << log_queue.size() << "\n";
                    for (const std::string &log : log_queue)
                    {
                        std::cout << log << "\n";
                    }
                    log_queue.clear();
                }
            }
        }
        ~Logger()
        {
            running = false;
        }
        void debug(std::string message)
        {
            std::lock_guard<std::mutex> guard(log_queue_lock);
            std::cout << "Thread adding debug message: " << std::this_thread::get_id() << "\n";
            log_queue.emplace_back(std::move(message));
        }
    protected:
    private:
        static std::atomic<bool> running;
        static std::mutex log_queue_lock;
        static std::vector<std::string> log_queue;
};

std::atomic<bool> Logger::running {true};
std::mutex Logger::log_queue_lock;
std::vector<std::string> Logger::log_queue {};


/*
static const short DEBUG {0x0};
static const short INFO  {0x1};
static const short WARN  {0x2};
static const short CRIT  {0x3};
*/

} // namespace p2p
#endif // P2P_LOGGER_

