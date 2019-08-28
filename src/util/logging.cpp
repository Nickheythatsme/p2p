//
// Created by Nick Grout on 2019-08-27.
//
#include "logging.h"

namespace p2p {
namespace util {

LogWriter::LogWriter()
{
    auto pred = [&, this]{return running == true;};
    std::condition_variable var;
    std::unique_lock<std::mutex> mute(logs_lock);
    logging_thread = std::move(std::thread(&LogWriter::output_logs, this, std::ref(var)));
    var.wait(mute, std::move(pred));
}

LogWriter::~LogWriter()
{
    running = false;
    logging_thread.join();
}

void LogWriter::add_logs(std::string log)
{
    std::lock_guard<std::mutex> lock_guard(logs_lock);
    logs.emplace_back(std::move(log));
}

void LogWriter::output_logs(std::condition_variable& ready)
{
    running = true;
    ready.notify_one();
    while (running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        std::lock_guard<std::mutex> lock_guard(logs_lock);
        if (!logs.empty()) {
            for (const auto& log : logs) {
                std::cout << log << '\n';
            }
            std::cout.flush();
            logs.clear();
        }
    }
}

} // namespace util
} // namespace p2p

