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
#include <fstream>

// Use chrono if we upgrade to c++20
#include <time.h> // for time formatting

namespace p2p {

#define OUTPUT_FREQ 100

using formatter_func = std::string (*)(const std::string& tag, const std::string& message);
std::string default_formatter(const std::string& tag, const std::string& message)
{
    // TODO add time formatting
    std::stringstream ss;
    ss << tag << ":" << message;
    return ss.str();
}


class NewLog
{
    public:
        NewLog(std::string message, std::shared_ptr<std::string> tag, formatter_func formatter):
            message(std::move(message)),
            tag(std::move(tag)),
            formatter(formatter)
        {
        }
        virtual ~NewLog() = default;
        friend std::ostream& operator<<(std::ostream& out, const NewLog& log)
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
        void add_log(NewLog &&message)
        {
            std::lock_guard<std::mutex> guard(new_logs_lock);
            new_logs.emplace_back(message);
        }
        std::streambuf* swap(std::streambuf *sb)
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
                std::vector<NewLog> to_output(std::move(new_logs));
                guard.unlock();

                for(const auto &message : to_output)
                {
                    *this << message << std::endl;
                }
            } while(running);
        }
    private:
        std::mutex new_logs_lock;
        std::vector<NewLog> new_logs;
        std::atomic<bool> running {true};
        std::thread logging_thread;
};

class Logger
{
    public:
        Logger() = default;
        ~Logger() = default;
        Logger(std::string tag, short log_level):
            tag(std::make_shared<std::string>(std::string(tag))),
            log_level(log_level)
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
        void use_stdout()
        {
            printer.swap(std::cout.rdbuf());
        }
        void use_file(const char* filename, bool append=false)
        {
            std::filebuf *fb = new std::filebuf;
            fb->open(filename, (append)?(std::ios_base::app):(std::ios_base::out));
            printer.swap(fb);
        }
        static const short DEBUG {0x0};
        static const short INFO  {0x1};
        static const short WARN  {0x2};
        static const short CRIT  {0x3};
    protected:
        void output_message(std::string &&message)
        {
            printer.add_log(
                NewLog(message, tag, formatter)
            );
        }
    private:
        formatter_func formatter {default_formatter};
        short log_level {Logger::DEBUG};
        std::shared_ptr<std::string> tag {std::make_shared<std::string>("tag")};
        static LoggerPrinter printer;
};

LoggerPrinter Logger::printer(nullptr);

} // namespace p2p
#endif // P2P_LOGGER_

