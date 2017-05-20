#include <iostream> 
#include <deque> //To store chat messages
#include <vector> //Same as up
#include <memory> //To use shared Pointer
#include <string>
#include <cstring>
#include <boost/asio.hpp> //TCP-IP Handling
#include <boost/array.hpp> //Boost::array


using boost::asio::ip::tcp;

int main(int argc,char * argv[]){
    try {
        std::string ip, name, msg;
        std::string quit = "quit";
        std::cout<<"Ingrese la IP a la que quiere conectarse: "<<std::endl;
        std::getline(std::cin,ip,'\n');
        std::cout<<"Ingrese su nombre: "<<std::endl;
        std::getline(std::cin,name,'\n');
        name.append(": ");
        quit.insert(0,name);
        

            //Creo el ioservice, resolver y el socket
            boost::asio::io_service io_service;
            tcp::resolver resolver(io_service); //el resolver se encarga de transformar los strings en ips y puertos
            tcp::socket socket(io_service);
    

            tcp::resolver::query listaendpoints(ip, "9001"); //se arma una lista con los posibles endpoints de esa ip en ese puerto

            tcp::resolver::iterator endpoints = resolver.resolve(listaendpoints); //Iteramos por la lista par aconseguir una ip y puerto al q podamos conectarnos


            boost::asio::connect(socket,endpoints); //Conectamos nuestro socket al primer endpoint posible

        do {
            std::cout<<"Ingrese el mensaje a enviar: "<<std::endl;
            std::getline(std::cin,msg,'\n');
            msg.insert(0,name);

            boost::system::error_code ignored_error;
            boost::asio::write(socket, boost::asio::buffer(msg),ignored_error); //Enviamos el mensaje por el socket

        } while(strcmp(quit.c_str(),msg.c_str()) != 0);//Si el usuario ingresa 'quit' se cierra el cliente
        socket.close();
        
    } catch (std::exception& e){ 
        std::cerr << e.what() << std::endl;
    }
    return 0;
};