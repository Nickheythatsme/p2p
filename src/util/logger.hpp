// 
// Nicholas Grout 4/30
//

#ifndef P2P_LOGGER_
#define P2P_LOGGER_
#include <string>
#include <condition_variable>
#include <mutex>
#include <iostream>
#include <thread>
#include <atomic>
#include <sstream>

// Use chrono if we upgrade to c++20
#include <time.h> // for time formatting

namespace p2p {

#define OUTPUT_FREQ 100

using formatter_func = std::ostream& (*)(std::ostream& out, const std::string& tag, const std::string& message);

std::ostream& default_formatter(std::ostream& out, const std::string &tag, const std::string &message)
{
    // TODO add time formatting
    return out << tag << ":" << message;
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
        Logger()
        {
        }
        ~Logger()
        {
        }
        void debug(std::string message)
        {
            output_message(std::move(message));
        }
        void info(std::string message)
        {
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
        short log_level;
        static LoggerPrinter printer;
};

LoggerPrinter Logger::printer(std::cout.rdbuf());


} // namespace p2p
#endif // P2P_LOGGER_

