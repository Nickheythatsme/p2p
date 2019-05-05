// 
// Nicholas Grout 4/30
//
#include <string>
#include <mutex>
#include <iostream>
#include <ostream>
#include <sstream>
#include <thread>
#include <atomic>
#include <fstream>
#include <map>

// Use chrono if we upgrade to c++20
#include <ctime> // for time formatting
#include <vector>

#ifndef P2P_LOGGER_
#define P2P_LOGGER_

namespace p2p
{

#define OUTPUT_FREQ 100

using formatter_func = std::string (*)(const std::string &tag, const std::string &message);
std::string
default_formatter(const std::string &tag, const std::string &message)
{
    // TODO add time formatting
    std::stringstream ss;
    const char *time_fmt = "%H:%M:%S";
    time_t current_time_t = time(nullptr);
    auto current_time_tm = gmtime(&current_time_t);
    char formatted_time[9];
    strftime(formatted_time, 9, time_fmt, current_time_tm);
    ss << "[" << formatted_time << "] [" << tag << "] " << message;
    return ss.str();
}

class NewLog
{
public:
    NewLog(std::string message, std::shared_ptr<std::string> tag, formatter_func formatter)
        :
        message(std::move(message)),
        tag(std::move(tag)),
        formatter(formatter)
    {
    }
    virtual ~NewLog() = default;
    friend std::ostream &
    operator<<(std::ostream &out, const NewLog &log)
    {
        out << log.formatter(*log.tag, log.message);
        return out;
    }
protected:
private:
    std::shared_ptr<std::string> tag;
    std::string message;
    formatter_func formatter;
};

class LoggerPrinter
{
public:
    explicit LoggerPrinter()
        :
        logging_thread(&LoggerPrinter::run, this)
    {
    }
    LoggerPrinter(const LoggerPrinter &rhs) = delete;
    LoggerPrinter(LoggerPrinter &&rhs) = delete;
    virtual ~LoggerPrinter()
    {
        stop();
    }
    void add_log(NewLog &&message)
    {
        std::lock_guard<std::mutex> guard(new_logs_lock);
        new_logs.emplace_back(message);
    }
    bool remove_ostream(const std::string &key)
    {
        std::lock_guard<std::mutex> output_guard(output_streams_lock);
        return output_streams.erase(key) > 0;
    }
    bool add_file(const char *filename, bool append)
    {
        using o_uptr = std::unique_ptr<std::ostream>;
        o_uptr fout(new std::ofstream(filename, (append) ? (std::ios_base::app) : (std::ios_base::out)));
        return add_ostream(filename, std::move(fout));
    }
    bool add_console()
    {
        using o_uptr = std::unique_ptr<std::ostream>;
        auto sb = std::cout.rdbuf(nullptr);
        return add_ostream("COUT CONSOLE", o_uptr(new std::ostream(sb)));
    }
protected:
    bool add_ostream(std::string &&key, std::unique_ptr<std::ostream> &&out)
    {
        std::lock_guard<std::mutex> output_guard(output_streams_lock);
        if (output_streams.count(key) > 0) {
            // Don't open the file because we're already logging to it
            return true;
        }
        else if (out->good()) {
            output_streams.insert(
                std::make_pair(std::move(key), std::move(out))
            );
            // Return true if success
            return true;
        }
        else {
            return false;
        }
    }
    void stop()
    {
        running = false;
        if (logging_thread.joinable())
            logging_thread.join();
    }
    void run()
    {
        do {
            // Wait for a period of time
            std::this_thread::sleep_for(std::chrono::milliseconds(OUTPUT_FREQ));
            std::unique_lock<std::mutex> guard(new_logs_lock);
            std::vector<NewLog> to_output(std::move(new_logs));
            guard.unlock();

            std::lock_guard<std::mutex> output_guard(output_streams_lock);
            for (auto &out : output_streams) {
                for (const auto &message : to_output) {
                    *out.second << message << std::endl;
                }
            }
        }
        while (running);
    }
private:
    std::mutex new_logs_lock;
    std::vector<NewLog> new_logs{};
    std::atomic<bool> running{true};
    std::thread logging_thread;
    std::mutex output_streams_lock;
    std::map<std::string, std::unique_ptr<std::ostream>> output_streams{};
};

class Logger
{
public:
    Logger() = default;
    ~Logger() = default;
    explicit Logger(const std::string &tag)
        :
        tag(std::make_shared<std::string>(tag))
    {
    }
    void debug(std::string message)
    {
        if (log_level <= Logger::DEBUG)
            output_message(std::move(message));
    }
    void info(std::string message)
    {
        if (log_level <= Logger::INFO)
            output_message(std::move(message));
    }
    void warn(std::string message)
    {
        if (log_level <= Logger::WARN)
            output_message(std::move(message));
    }
    void crit(std::string message)
    {
        output_message(std::move(message));
    }
    static void set_formatter(formatter_func func)
    {
        formatter = func;
    }
    static bool add_file(const char *filename, bool append = true)
    {
        return printer.add_file(filename, append);
    }
    static bool use_console()
    {
        return printer.add_console();
    }
    static const short DEBUG;
    static const short INFO;
    static const short WARN;
    static const short CRIT;
    static short log_level;
protected:
    void
    output_message(std::string &&message)
    {
        printer.add_log(
            NewLog(std::move(message), tag, formatter)
        );
    }
private:
    static formatter_func formatter;
    std::shared_ptr<std::string> tag{std::make_shared<std::string>("LOG")};
    static LoggerPrinter printer;
};

const short Logger::DEBUG{0x0};

const short Logger::INFO{0x1};

const short Logger::WARN{0x2};

const short Logger::CRIT{0x3};

LoggerPrinter Logger::printer;

short Logger::log_level{Logger::DEBUG};

formatter_func Logger::formatter{default_formatter};

} // namespace p2p
#endif // P2P_LOGGER_

