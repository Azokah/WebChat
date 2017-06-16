#include <iostream>
#include <boost/asio.hpp>
#include "ServerChat.h"

int main(int argc, char *argv[]){
    try{
        boost::asio::io_service io_service;
        ServerChat *server = new ServerChat(io_service);
        //io_service.run();
        server->listen();
    }
    catch (std::exception &e){
        std::cerr << "Exception: " << e.what() << "\n";
    }
    return 0;
}