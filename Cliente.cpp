#include <iostream> 
#include <deque> //To store chat messages
#include <vector> //Same as up
#include <memory> //To use shared Pointer
#include <boost/asio.hpp> //TCP-IP Handling
#include <boost/array.hpp> //Boost::array


using boost::asio::ip::tcp;

int main(int argc,char * argv[]){
    try {
        boost::asio::io_service io_service; //Always needs to be a io_service in boost
        tcp::resolver resolver(io_service); // We resolve the server name into a TCP endpoint
        tcp::resolver::query query(argv[1], "daytime"); // We use the name of the server and the name of the service
        //To make a query for the resolver

        tcp::resolver::iterator endpoint_iterator = resolver.resolve(query); // We iterate trough the query with the resolver
    
        //Now we create and connect the socket. The list of endpoints obtained above may contain both IPv4 and IPv6 endpoints,
        //so we need to try each of them until we find one that works. 
        tcp::socket socket(io_service);
        boost::asio::connect(socket, endpoint_iterator);
        //The connection is Open.

        //We use a std::vector to hold the received data.
        //The boost::asio::buffer() function automatically determines the size of the array to help prevent buffer overruns. 
        while(true){
            boost::array<char, 128> buffer;
            boost::system::error_code error;

            size_t len = socket.read_some(boost::asio::buffer(buffer), error);

            if (error == boost::asio::error::eof)
                break; // Connection closed cleanly by peer.
            else if (error)
                throw boost::system::system_error(error); // Some other error.

            std::cout.write(buffer.data(), len);
        }
    } catch (std::exception& e){ 
        std::cerr << e.what() << std::endl;
    }
    
    
    return 0;
};