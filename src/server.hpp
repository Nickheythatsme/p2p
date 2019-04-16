/*
 * Nicholas Grout 4/16/2019
 *
 * TODO implement full server (this is just an example)
 */


#include <ctime>
#include <iostream>
#include <string>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

namespace p2p {

class server
{
    public:
        std::string make_daytime_string()
        {
          using namespace std; // For time_t, time and ctime;
          time_t now = time(0);
          return ctime(&now);
        }

        void listen(unsigned short port)
        {
            try
            {
                boost::asio::io_context io_context;

                tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), port));

                for (;;)
                {
                    tcp::socket socket(io_context);
                    acceptor.accept(socket);

                    std::string message = make_daytime_string();

                    boost::system::error_code ignored_error;
                    boost::asio::write(socket, boost::asio::buffer(message), ignored_error);
                }
            }
            catch (std::exception& e)
            {
                std::cerr << e.what() << std::endl;
            }
        }
    protected:
    private:
};

} //namespace p2p
