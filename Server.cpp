#include <ctime>
#include <iostream>
#include <string>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

std::string make_daytime_string()
{
  using namespace std; // For time_t, time and ctime;
  time_t now = time(0);
  return ctime(&now);
}

int main()
{
    try {
        boost::asio::io_service io_service;

        //Connection Listener, initialized on port 13 and IPv4
        tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 13));

        //This is an iterative server, which means that it will handle one connection at a time. 
        //Create a socket that will represent the connection to the client, and then wait for a connection. 
        while(true){
            tcp::socket socket(io_service);
            acceptor.accept(socket);

            //A client is accessing our service. Determine the current time and transfer this information to the client. 
            std::string message = make_daytime_string();

            boost::system::error_code ignored_error;
            boost::asio::write(socket, boost::asio::buffer(message), ignored_error);
        }
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
  return 0;
}