#include "ClienteChat.h"

ClienteChat::ClienteChat(){
    std::cout<<"Ingrese la IP a la que quiere conectarse: "<<std::endl;
    std::getline(std::cin,ip,'\n');
    std::cout<<"Ingrese su nombre: "<<std::endl;
    std::getline(std::cin,name,'\n');
    quit = "quit";
    name.append(": ");
    quit.insert(0,name);
};
/*ClienteChat::ClienteChat(std::string ipa, std::string namea) : ip(ipa), name(namea){
    quit = "quit";
    name.append(": ");
    quit.insert(0,name);
};*/

void ClienteChat::conexionHandler(const boost::system::error_code& ec, tcp::resolver::iterator i){};

void ClienteChat::handlerResolver(const boost::system::error_code& ec,tcp::resolver::iterator i){
    if (!ec){
        boost::asio::async_connect(*socket, i, boost::bind(&ClienteChat::conexionHandler,
                                                this,_1,
                                                _2));
    }
}


//boost::bind(&BoostTCPConnection::handleReceiveMessage, shared_from_this()
//,boost::asio::placeholders::error, message)
void ClienteChat::start(){
    try {
        resolver = new tcp::resolver(io_service); //el resolver se encarga de transformar los strings en ips y puertos
        socket = new tcp::socket(io_service);
    
        listaendpoints = new tcp::resolver::query (ip, "9001"); //se arma una lista con los posibles endpoints de esa ip en ese puerto
        //endpoints = new tcp::resolver::iterator(); //Iteramos por la lista par aconseguir una ip y puerto al q podamos conectarnos
        resolver->async_resolve(*listaendpoints,boost::bind(&ClienteChat::handlerResolver,this,_1,_2));//,boost::bind(&ClienteChat::handlerResolver, this));
        //boost::asio::connect(*socket,*endpoints); //Conectamos nuestro socket al primer endpoint posible
        msg.append(name);
        msg.append("se ha conectado al servidor.");
        boost::system::error_code ignored_error;
        boost::asio::write(*socket, boost::asio::buffer(msg),ignored_error); //Enviamos el mensaje por el socket
        chat();
    } catch (std::exception& e){ 
        std::cerr << e.what() << std::endl;
    }
}

void ClienteChat::chat(){
    do {
            std::cout<<"Ingrese el mensaje a enviar: "<<std::endl;
            std::getline(std::cin,msg,'\n');
            msg.insert(0,name);

            boost::system::error_code ignored_error;
            boost::asio::write(*socket, boost::asio::buffer(msg),ignored_error); //Enviamos el mensaje por el socket

        } while(strcmp(quit.c_str(),msg.c_str()) != 0);//Si el usuario ingresa 'quit' se cierra el cliente
        socket->close();
}

