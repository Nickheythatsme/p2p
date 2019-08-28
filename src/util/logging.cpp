//
// Created by Nick Grout on 2019-08-27.
//
#include "logging.h"

#include <memory>

namespace p2p {
namespace util {

std::mutex Logger::log_writer_lock;
std::unique_ptr<LogWriter> Logger::log_writer = nullptr;

LogWriter::LogWriter()
{
    auto pred = [&, this]{return running == true;};
    std::unique_lock<std::mutex> mute(logs_lock);
    logging_thread = std::move(std::thread(&LogWriter::output_logs, this));
    logs_ready.wait(mute, std::move(pred));
    printf("constructor finished\n");
}

LogWriter::~LogWriter()
{
    running = false;
    logging_thread.join();
}

template<typename T>
LogWriter& LogWriter::write(const T& t)
{
    printf("LogWriter::write\n");
    std::lock_guard<std::mutex> lock_guard(logs_lock);
    logs << t << '\n';
    logs_to_write = true;
    logs_ready.notify_one();
    return *this;
}

template<typename T>
const T& LogWriter::_write(const T& t)
{
    return t;
}

template<typename T, typename ...Args>
LogWriter& LogWriter::write(const T& t, Args ...args)
{
    std::stringstream ss;
    ss << t << _write(args...);
    return write(ss.str());
}

void LogWriter::output_logs()
{
    running = true;
    // tell constructor we're set up so it can stop blocking
    logs_ready.notify_one();

    auto pred = [&, this]{return logs_to_write == true;};
    std::unique_lock<std::mutex> mute(logs_lock);
    while (running) {
        logs_ready.wait(mute, pred);
        std::cout << logs.str();
        logs = std::move(std::stringstream());
        std::cout.flush();
        logs_to_write = false;
    }
}

// LOGGER
void Logger::init_log_writer()
{
    std::lock_guard<std::mutex> lock_guard(Logger::log_writer_lock);
    if (!log_writer) {
        log_writer = std::make_unique<LogWriter>();
    }
}

Logger::Logger()
{
    init_log_writer();
}

Logger::Logger(std::string tag):
    tag(std::move(tag))
{
    init_log_writer();
}

template<typename ...T>
Logger& Logger::write(T ...t)
{
    Logger::log_writer->write(t...);
}

} // namespace util
} // namespace p2p

