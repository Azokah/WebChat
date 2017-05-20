#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <boost/asio.hpp>
#include <boost/array.hpp> //Boost::array

using boost::asio::ip::tcp;

int main()
{
    std::vector<std::string> mensajesLog;
    std::vector<tcp::socket> sockets;
    try {
        boost::array<char,128> msg;

        boost::asio::io_service io_service; //io_service
        //Connection Listener, initialized on port 8000 and IPv4
        tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 9001));
        
        std::cout<<"Escuchando..."<<std::endl;
        while(true){
            tcp::socket socket(io_service); 
            acceptor.accept(socket);
            std::cout<<"Conectado.."<<std::endl;
            boost::system::error_code error;
            while(true){
                size_t tam = socket.read_some(boost::asio::buffer(msg), error);
                std::cout.write(msg.data(), tam);
                mensajesLog.push_back(msg.data());
                std::cout<<std::endl;

                //if (!error == boost::asio::error::eof) std::cerr<<"Error"; esto es que si ya no llega info tira error
            }
            socket.close();
        }
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}