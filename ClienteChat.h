#pragma once
#ifndef CLIENTECHATH
#define CLIENTECHATH
#include <iostream> 
#include <deque> //To store chat messages
#include <vector> //Same as up
#include <memory> //To use shared Pointer
#include <string>
#include <cstring>
#include <boost/asio.hpp> //TCP-IP Handling
#include <boost/array.hpp> //Boost::array
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>

using boost::asio::ip::tcp;

class ClienteChat {
    public:
        ClienteChat();
        //ClienteChat(std::string, std::string);
        ~ClienteChat();

        void start();
        void chat();

        void conexionHandler(const boost::system::error_code& ec, tcp::resolver::iterator i);
        void handlerResolver(const boost::system::error_code& ec,tcp::resolver::iterator i);
        

    private:
        std::string ip, name, msg, quit;

        boost::asio::io_service io_service;
        tcp::resolver * resolver; //el resolver se encarga de transformar los strings en ips y puertos
        tcp::socket * socket;
        tcp::resolver::query * listaendpoints;
        tcp::resolver::iterator * endpoints;

        
        /*void handlerResolver(const boost::system::error_code& ec,
        tcp::resolver::iterator i);*/

        
};

#endif /* CLIENTECHATH*/