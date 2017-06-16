#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <SFML/Network.hpp>
#define DATA_SIZE 100
#define PUERTO 53000

void mandarData(sf::TcpSocket * socket,std::string * mensaje){
    const char * data;
    data = mensaje->c_str();
    if (socket->send(data, mensaje->size()) != sf::Socket::Done)
    {
       std::cout<<"Error enviando data.."<<std::endl;
    }
};

void retransmitir(sf::TcpSocket * socket,char * data){
    if (socket->send(data,DATA_SIZE) != sf::Socket::Done)
    {
       std::cout<<"Error enviando data.."<<std::endl;
    }
};

void recibirData(sf::TcpSocket * socket){
    char data[DATA_SIZE];
    std::size_t bytesRecibidos;

    if (socket->receive(data, DATA_SIZE, bytesRecibidos) != sf::Socket::Done)
    {
       std::cout<<"Error recibiendo data.."<<std::endl;
    }
    std::cout<<data<<" - "<<"recibido en "<<bytesRecibidos<<" bytes."<<std::endl;
    retransmitir(socket,data);
};

void aceptarConexiones(sf::TcpListener * listener, std::vector<sf::TcpSocket*> * clientes,sf::SocketSelector * socketSelector){
    sf::TcpSocket * socket;

    if (listener->listen(PUERTO) != sf::Socket::Done){
        std::cout<<"Error escuchando conexiones.."<<std::endl;
    }else{
        //Aceptando clientes
        if (listener->accept(*socket) != sf::Socket::Done){
            std::cout<<"Error aceptando conexiones.."<<std::endl;
        }else{
            clientes->push_back(socket);
            socketSelector->add(*socket);
        }
    }
}

int main(int argc, char *argv[])
{
    std::vector<sf::TcpSocket*> clientes;
    sf::TcpSocket cliente;
    sf::IpAddress ip = sf::IpAddress::getLocalAddress();
    sf::TcpListener listener;
    unsigned short puerto = PUERTO;
    sf::SocketSelector socketSelector;
    sf::SocketSelector listenerSelector;
    

    // bindear el listener con un puerto para escuchar conexiones
    
    while(true){
        if(socketSelector.wait(sf::seconds(2))){
            for(int i = 0; i < clientes.size(); i++){
                if(socketSelector.isReady(*clientes.at(i))){
                    recibirData(clientes.at(i));
                }
            }
        }
        else
        {
            std::cout<<"No se recibio nada..."<<std::endl;
            aceptarConexiones(&listener,&clientes,&socketSelector);
        }
    }
    /*
    if (socketSelector.wait(sf::seconds(2))){
        for(sf::TcpSocket socket : clientes){ //Aca necesito hacer un vector de sockets
            if (socketSelector.isReady(socket)){
                // this socket is ready, you can receive (or accept if it's a listener)
                recibirData(&socket);
            }
        }
    }else{

    }
    */

    return 0;
}