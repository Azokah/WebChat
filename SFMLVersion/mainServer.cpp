#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <SFML/Network.hpp>
#define DATA_SIZE 100
#define PUERTO 53000



/* Por Jaime Gonzalez Bonorino
Legajo: 90892*/


void mandarData(sf::TcpSocket * socket,std::string * mensaje){
    const char * data = "";
    data = mensaje->c_str();
    if (socket->send(data, mensaje->size()) != sf::Socket::Done)
    {
       std::cout<<"Error enviando data.."<<std::endl;
    }
};

void retransmitir(std::vector<sf::TcpSocket *> * socket,char * data,std::size_t bytes){
    for(int i = 0; i < socket->size(); i++){
        if (socket->at(i)->send(data,bytes) != sf::Socket::Done)
        {
        std::cout<<"Error enviando data.."<<std::endl;
        }
    }
};

void recibirData(sf::TcpSocket * socket, std::vector<sf::TcpSocket*> * sockets){
    char data[DATA_SIZE] = {0};
    std::size_t bytesRecibidos;

    if (socket->receive(data, DATA_SIZE, bytesRecibidos) != sf::Socket::Done)
    {
       std::cout<<"Error recibiendo data.."<<std::endl;
    }
    std::cout<<data<<" - "<<"recibido en "<<bytesRecibidos<<" bytes."<<std::endl;
    retransmitir(sockets,data,bytesRecibidos);
};

void aceptarConexiones(sf::TcpListener * listener,sf::SocketSelector * listenerSelector, std::vector<sf::TcpSocket*> * clientes,sf::SocketSelector * socketSelector){
    sf::TcpSocket * socket = new sf::TcpSocket();
    socket->setBlocking(false);
    if (listener->accept(*socket) != sf::Socket::Done){
            //std::cout<<"Error aceptando conexiones.."<<std::endl;
    }else{
            clientes->push_back(socket);
            socketSelector->add(*socket);
    }


};
void aceptarConexionPrimera(sf::TcpListener * listener, std::vector<sf::TcpSocket*> * clientes,sf::SocketSelector * socketSelector){
    sf::TcpSocket * socket = new sf::TcpSocket();

    if (listener->listen(PUERTO) != sf::Socket::Done){
        std::cout<<"Error escuchando conexiones.."<<std::endl;
    }else{
        //Aceptando clientes
        if (listener->accept(*socket) != sf::Socket::Done){
            std::cout<<"Error aceptando conexiones.."<<std::endl;
        }else{
            clientes->push_back(socket);
            socketSelector->add(*socket);
            listener->setBlocking(false);
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
    listenerSelector.add(listener);

    aceptarConexionPrimera(&listener,&clientes,&socketSelector);

    while(true){
        if(socketSelector.wait(sf::seconds(2))){
            for(int i = 0; i < clientes.size(); i++){
                if(socketSelector.isReady(*clientes.at(i))){
                    recibirData(clientes.at(i),&clientes);
                }
            }
        }
        else
        {
            //std::cout<<"No se recibio nada..."<<std::endl;
            aceptarConexiones(&listener,&listenerSelector,&clientes,&socketSelector);
        }
    }

    return 0;
}