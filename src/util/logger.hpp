// 
// Nicholas Grout 4/30
//

#ifndef P2P_LOGGER_
#define P2P_LOGGER_
#include <string>
#include <mutex>
#include <ostream>
#include <thread>
#include <atomic>
#include <sstream>

// Use chrono if we upgrade to c++20
#include <time.h> // for time formatting

namespace p2p {

#define OUTPUT_FREQ 100

using formatter_func = std::string (*)(const std::string& tag, const std::string& message);

struct new_log
{
    std::string message;
    const std::string* tag;
    formatter_func formatter;
};

std::string default_formatter(const std::string& tag, const std::string& message)
{
    // TODO add time formatting
    std::stringstream ss;
    ss << tag << ":" << message;
    return ss.str();
}

class LoggerPrinter: public std::ostream
{
    public:
        explicit LoggerPrinter(std::streambuf *sb):
            std::ostream(sb),
            logging_thread(&LoggerPrinter::run, this)
        {
        }
        LoggerPrinter(const LoggerPrinter &rhs) = delete;
        LoggerPrinter(LoggerPrinter &&rhs) = delete;
        virtual ~LoggerPrinter()
        {
            stop();
        }
        void add_log(std::string message)
        {
            std::unique_lock<std::mutex> guard(new_logs_lock);
            new_logs.emplace_back(std::move(message));
            guard.unlock();
        }
        std::streambuf* swap(std::streambuf* sb)
        {
            std::lock_guard<std::mutex> guard(new_logs_lock);
            return this->rdbuf(sb);
        }
    protected:
        void stop()
        {
            running=false;
            if (logging_thread.joinable())
                logging_thread.join();
        }
        void run()
        {
            do {
                std::this_thread::sleep_for(std::chrono::milliseconds(OUTPUT_FREQ));
                std::unique_lock<std::mutex> guard(new_logs_lock);
                std::vector<std::string> to_output(std::move(new_logs));
                guard.unlock();

                for(const std::string &message : to_output)
                {
                    *this << message << std::endl;
                }
            } while(running);
        }
    private:
        std::mutex new_logs_lock;
        std::vector<std::string> new_logs;
        std::atomic<bool> running {true};
        std::thread logging_thread;
};

class Logger
{
    public:
        Logger() = default;
        ~Logger() = default;
        Logger(std::string tag, short log_level):
            tag(std::move(tag)),
            log_level(log_level)
        {
        }
        void debug(std::string message)
        {
            if (log_level >= Logger::DEBUG)
                output_message(std::move(message));
        }
        void info(std::string message)
        {
            if (log_level >= Logger::INFO)
                output_message(std::move(message));
        }
        void warn(std::string message)
        {
            if (log_level >= Logger::WARN)
                output_message(std::move(message));
        }
        void crit(std::string message)
        {
            output_message(std::move(message));
        }
        std::streambuf* swap(std::streambuf* sb)
        {
            return printer.swap(sb);
        }
        static const short DEBUG {0x0};
        static const short INFO  {0x1};
        static const short WARN  {0x2};
        static const short CRIT  {0x3};
    protected:
        void output_message(std::string message)
        {
            printer.add_log(std::move(message));
        }
    private:
        short log_level {Logger::INFO};
        std::string tag;
        static LoggerPrinter printer;
};

LoggerPrinter Logger::printer(nullptr);

} // namespace p2p
#endif // P2P_LOGGER_

