// 
// Nicholas Grout 4/30
//

#ifndef P2P_LOGGER_
#define P2P_LOGGER_
#include <memory>
#include <iostream>

namespace p2p {

// Shorthand for the desired basic_ostream
using _ostream_type = std::basic_ostream<char, std::char_traits<char>>;
using _ptr_ostream_type = std::shared_ptr<_ostream_type>;
using _log_level = unsigned short;

class LoggerBase
{
    public:
        LoggerBase() = default;
        virtual ~LoggerBase() { }

        virtual LoggerBase& operator<<(const char *message) { return *this; }
        virtual LoggerBase& operator<<(char message) { return *this; }
        virtual LoggerBase& operator<<(int message) { return *this; }
        virtual LoggerBase& operator<<(unsigned int message) { return *this; }
        virtual LoggerBase& operator<<(double message) { return *this; }
        virtual LoggerBase& operator<<(const void* address) { return *this; }
    protected:
    private:
};
 
class Logger: public LoggerBase
{
    public:
        Logger(_ostream_type ostream) :
            LoggerBase(),
            os(ostream)
        {
        }
        LoggerBase& operator<<(const char *message)
        {
            return output(message);
        }
        LoggerBase& operator<<(char message)
        {
            return output(message);
        }
        LoggerBase& operator<<(int message)
        {
            return output(message);
        }
        LoggerBase& operator<<(unsigned int message)
        {
            return output(message);
        }
        LoggerBase& operator<<(double message)
        {
            return output(message);
        }
        LoggerBase& operator<<(const void* address)
        {
            return output(address);
        }

        template<class _mType>
        Logger& output(const _mType& message)
        {
            *os << message << std::endl;
            return *this;
        }

        /*
        static const short DEBUG {0x0};
        static const short INFO  {0x1};
        static const short WARN  {0x2};
        static const short CRIT  {0x3};
        */
    protected:
        _ptr_ostream_type os;
    private:
};

} // namespace p2p
#endif // P2P_LOGGER_

