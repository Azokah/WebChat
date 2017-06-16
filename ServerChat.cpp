#include "ServerChat.h"


ServerChat::ServerChat(boost::asio::io_service& service):io_service(service){
    //Connection Listener, initialized on port 8000 and IPv4
    acceptor = new tcp::acceptor(io_service, tcp::endpoint(tcp::v4(), 9001));
    socket = new tcp::socket (io_service); 
};
ServerChat::~ServerChat(){};

void ServerChat::manejarLectura(){
    socket->async_read_some(boost::asio::buffer(msg, msg.size()),
    boost::bind(&ServerChat::handlerLectura,this,
    boost::asio::placeholders::error,
    boost::asio::placeholders::bytes_transferred));

    std::cout.write(msg.data(), msg.size());
    mensajesLog.push_back(msg.data());
    std::cout<<std::endl;
}
void ServerChat::manejarEscritura(){
    socket->async_write_some(boost::asio::buffer(msg, msg.size()),
    boost::bind(&ServerChat::handlerEscritura,this,
    boost::asio::placeholders::error,
    boost::asio::placeholders::bytes_transferred));
};

void ServerChat::listen(){
    try {
            std::cout<<"Esperando clientes..."<<std::endl;
            while(true){
                
                //acceptor->accept(*socket);
                acceptor->async_accept(*socket, handlerAccept);
                read();
            }
            socket->close();
            
        } catch (std::exception& e) {
            std::cerr << e.what() << std::endl;
    }
};
void ServerChat::read(){
    while(true){
        size_t tam = socket->read_some(boost::asio::buffer(msg), error);
        if(msg.data() != NULL){
            std::cout.write(msg.data(), tam);
            mensajesLog.push_back(msg.data());
            std::cout<<std::endl;
        }

        //if (!error == boost::asio::error::eof) std::cerr<<"Error"; esto es que si ya no llega info tira error
    }
    
};