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
void recibirData(sf::TcpSocket * socket){
    char data[DATA_SIZE] = {0};
    std::size_t bytesRecibidos;

    if (socket->receive(data, DATA_SIZE, bytesRecibidos) != sf::Socket::Done)
    {
       std::cout<<"Error recibiendo data.."<<std::endl;
    }
    std::cout<<data<<" - "<<"recibido en "<<bytesRecibidos<<" bytes."<<std::endl;
};

void init(std::string * ip, std::string * name, std::string * mensaje){
    std::cout<<"Ingrese la IP a la que quiere conectarse: "<<std::endl;
    std::getline(std::cin,*ip,'\n');
    std::cout<<"Ingrese su nombre: "<<std::endl;
    std::getline(std::cin,*name,'\n');
    name->append(": ");
}

void mensajear(sf::TcpSocket * socket, std::string *name){
    std::string mensaje;
    std::cout<<"Ingrese su mensaje: "<<std::endl;
    std::getline(std::cin,mensaje,'\n');
    mensaje.insert(0,*name);
    mandarData(socket,&mensaje);
}

int main(int argc, char *argv[])
{
    std::string ip, name, msg;
    std::string quit = "quit";
    init(&ip,&name,&msg);
    quit.insert(0,name);
    sf::TcpSocket socket;
    sf::IpAddress ipA(ip);
    sf::SocketSelector socketSelector;

    sf::Socket::Status status = socket.connect(ipA, PUERTO);
    if (status != sf::Socket::Done)
    {
        std::cout<<"No se pudo conectar con server.."<<std::endl;
    }else{
        socketSelector.add(socket);
        while(true){
            if (socketSelector.wait(sf::seconds(2))){
                if (socketSelector.isReady(socket)){
                    recibirData(&socket);
                }
            }else{
                mensajear(&socket,&name);  
            }
        }
    }
    socket.disconnect();

    return 0;
}