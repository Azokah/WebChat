#include "ServerChat.h"


ServerChat::ServerChat(){};
ServerChat::~ServerChat(){};

void ServerChat::listen(){
    try {
            //Connection Listener, initialized on port 8000 and IPv4
            acceptor = new tcp::acceptor(io_service, tcp::endpoint(tcp::v4(), 9001));
        
            std::cout<<"Esperando clientes..."<<std::endl;
            while(true){
                socket = new tcp::socket (io_service); 
                acceptor->accept(*socket);

                read();

                socket->close();
            }
            
        } catch (std::exception& e) {
            std::cerr << e.what() << std::endl;
    }
};
void ServerChat::read(){
    while(true){
        size_t tam = socket->read_some(boost::asio::buffer(msg), error);
        std::cout.write(msg.data(), tam);
        mensajesLog.push_back(msg.data());
        std::cout<<std::endl;

        //if (!error == boost::asio::error::eof) std::cerr<<"Error"; esto es que si ya no llega info tira error
    }
    
};