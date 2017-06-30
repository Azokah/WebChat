#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <SFML/Network.hpp>
#define DATA_SIZE 100
#define PUERTO_SERVER 53000 //PUERTO POR EL CUAL NOS CONECTAMOS AL SERVIDOR DE LOOKBACK
#define PUERTO_CHAT 53001 // PUERTO POR EL CUAL LOS CLIENTES Y SERVIDORES INTERACTUAN

/* Por Jaime Gonzalez Bonorino
Legajo: 90892*/


//PEDAZO DE CODIGO DE SERVER QUE NO SABIA DONDE METER 
//En realidad no sabia como declarar el prototipo de un struct
struct Server{
    std::vector<sf::TcpSocket*> clientes;
    sf::TcpSocket cliente;
    sf::IpAddress ip = sf::IpAddress::getLocalAddress();
    sf::TcpListener listener;
    unsigned short puerto = PUERTO_CHAT;
    sf::SocketSelector socketSelector;
    sf::SocketSelector listenerSelector;
    //La primera conexion es bloqueante                                                            //El resto no.
};

void initServer(struct Server * server);
void loopServer(struct Server * server);

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

void recibirDirecciones(sf::TcpSocket * socket, std::vector<sf::IpAddress> * direcciones){
    char data[DATA_SIZE] = {0};
    std::size_t bytesRecibidos;
    bool encontrado = false;

    if (socket->receive(data, DATA_SIZE, bytesRecibidos) != sf::Socket::Done)
    {
       std::cout<<"Error recibiendo data.."<<std::endl;
    }
    std::cout<<data<<" - "<<"recibido en "<<bytesRecibidos<<" bytes."<<std::endl;
    if(bytesRecibidos <= 15){
        for(int i = 0; i < direcciones->size(); i ++){
            if(direcciones->at(i) == data){
                encontrado = true;
            }
        }
        if(!encontrado){
            std::string ipNueva = data;
            sf::IpAddress ip(ipNueva);
            direcciones->push_back(ip);
            std::cout<<"Agregada nueva IP"<<std::endl;
        }
    }else{
        std::cout<<"Error recibiendo una de las direcciones"<<std::endl;
    }
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

void conectaraClientes(std::vector<sf::TcpSocket> * clientes, std::vector<sf::IpAddress> * direcciones,int * conectados){
    for(int i = *conectados; i < direcciones->size();i++){
        sf::Socket::Status status = clientes->at(i).connect(direcciones->at(i), PUERTO_CHAT);
        if (status != sf::Socket::Done){
            std::cout<<"No se pudo conectar con server: "<<direcciones->at(i)<<std::endl;
        }else{
            std::cout<<"Direccion "<<direcciones->at(i)<<" conectada."<<std::endl;
            (*conectados)++;
        }
    }
};
int main(int argc, char *argv[]){

    std::string ip, name, msg;
    std::string quit = "quit";
    init(&ip,&name,&msg);
    quit.insert(0,name);
    sf::TcpSocket socket;
    sf::IpAddress ipA(ip);
    sf::SocketSelector socketSelector;
    std::vector<sf::IpAddress> direcciones;
    std::vector<sf::TcpSocket> socketClientes;
    int conecciones = 0;

    struct Server * server = (struct Server*)malloc(sizeof(struct Server)); //Generamos la instancia de 'server'
    initServer(server); //La inicializamos

    sf::Socket::Status status = socket.connect(ipA, PUERTO_SERVER); //Primero conectamos con el server de lookback
    if (status != sf::Socket::Done)
    {
        std::cout<<"No se pudo conectar con server.."<<std::endl;
    }else{
        socketSelector.add(socket);
        while(true){
            if (socketSelector.wait(sf::seconds(2))){
                if (socketSelector.isReady(socket)){
                    recibirDirecciones(&socket,&direcciones); //Cada dos segundos se reciben las direcciones del server de lookback
                }
            }else{
                mensajear(&socket,&name);  //Escribimos nuestro mensaje
                conectaraClientes(&socketClientes,&direcciones,&conecciones); //Conectamos a clientes nuevos si los hay(al recbir las nuevas direcciones)
                loopServer(server);//Hacemos un loop del server propio (es decir, recibimos los mensajes que nos han enviado otros clientes)
                
            }
        }
    }
    socket.disconnect();

    return 0;
}
//De aca para abajo es todo codigo del (auto)server
//LLamo auto server a cada cliente que abre una instancia de servidor, para que pueda recibir mensajes
//de otros clientes. Cada cliente envia cadenas a otros servidores, los servidores los reciben y muestran
//De esta forma, un cliente enrealidad es una mezcla entre "cliente y servidor"


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
    std::cout<<data<<std::endl;//" - "<<"recibido en "<<bytesRecibidos<<" bytes."<<std::endl;
    //retransmitir(sockets,data,bytesRecibidos);
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

    if (listener->listen(PUERTO_CHAT) != sf::Socket::Done){
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


void initServer(struct Server * server){
    
    server->listenerSelector.add(server->listener);

    aceptarConexionPrimera(&server->listener,&server->clientes,&server->socketSelector);    
}

void loopServer(struct Server * server){
    //while(true){
        if(server->socketSelector.wait(sf::seconds(2))){ //Si algun socket va a recibir algo(esperar 2 segundos)
            for(int i = 0; i < server->clientes.size(); i++){
                if(server->socketSelector.isReady(*server->clientes.at(i))){
                    recibirData(server->clientes.at(i),&server->clientes); //Imprime la data que recibe el servidor
                }
            }
        }
        else
        {
            //std::cout<<"No se recibio nada..."<<std::endl;
            aceptarConexiones(&server->listener,&server->listenerSelector,&server->clientes,&server->socketSelector); //Acepta nuevas conexiones
        }
    //}
};