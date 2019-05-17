#include <memory>

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
    void add_log(std::string&& message)
    {
        std::lock_guard<std::mutex> guard(new_logs_lock);
        new_logs.emplace_back(std::move(message));
    }
    void crit_log(std::string&& message)
    {
        std::lock_guard<std::mutex> output_guard(output_streams_lock);
        for (auto &out : output_streams) {
                *out.second << message << std::endl;
        }
    }
    bool remove_ostream(const std::string &key)
    {
        std::lock_guard<std::mutex> output_guard(output_streams_lock);
        return output_streams.erase(key) > 0;
    }
    bool add_file(const char *filename, bool append)
    {
        std::unique_ptr<std::ostream> fout(new std::ofstream(filename, (append) ? (std::ios_base::app) : (std::ios_base::out)));
        return add_ostream(filename, std::move(fout));
    }
    bool add_console()
    {
        auto sb = std::cout.rdbuf(nullptr);
        return add_ostream("COUT CONSOLE", std::make_unique<std::ostream>(sb));
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
            auto to_output(std::move(new_logs));
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
    std::vector<std::string> new_logs{};
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
    explicit Logger(std::string tag):
        tag(std::move(tag))
    {
    }
    template <typename... Ts>
    void debug(Ts const&...messages)
    {
        if (log_level <= Logger::DEBUG)
            output_message(Logger::DEBUG, messages...);
    }
    template <typename... Ts>
    void info(Ts const&... messages)
    {
        if (log_level <= Logger::INFO)
            output_message(Logger::INFO, messages...);
    }
    template <typename... Ts>
    void warn(Ts const&... messages)
    {
        if (log_level <= Logger::WARN)
            output_message(Logger::WARN, messages...);
    }
    template <typename... Ts>
    void crit(Ts const&... messages)
    {
        output_message(Logger::CRIT, messages...);
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
    static const char* level_names[];
protected:
    template<typename ...Ts>
    void output_message(short level, Ts const &... rest)
    {
        std::stringstream ss;
        add_log_header(ss, level);
        _accumulate_args(ss, rest...);
        if (level == Logger::CRIT)
            printer.crit_log(ss.str());
        else
            printer.add_log(ss.str());
    }
    // Create standard message
    std::ostream& add_log_header(std::stringstream& ss, short level)
    {
        auto time_fmt = "%H:%M:%S";
        time_t current_time_t = time(nullptr);
        auto current_time_tm = gmtime(&current_time_t);
        char formatted_time[9];
        strftime(formatted_time, 9, time_fmt, current_time_tm);

        ss << "[" << formatted_time << "][" << tag << "][" << Logger::level_names[level] << "] ";
        return ss;
    }
    template<typename Arg, typename ...Ts>
    void _accumulate_args(std::stringstream& ss, const Arg& arg, Ts const&...rest)
    {
        ss << arg;
        _accumulate_args(ss, rest...);
    }
    template<typename Arg>
    void _accumulate_args(std::stringstream& ss, const Arg& arg)
    {
        ss << arg;
    }
private:
    std::string tag{"LOG"};
    static LoggerPrinter printer;
};

const short Logger::DEBUG{0x0};
const short Logger::INFO{0x1};
const short Logger::WARN{0x2};
const short Logger::CRIT{0x3};

LoggerPrinter Logger::printer;
const char* Logger::level_names[] = {
    "D",
    "I",
    "W",
    "C"
};
short Logger::log_level{Logger::DEBUG};

} // namespace p2p
#endif // P2P_LOGGER_

