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

using boost::asio::ip::tcp;

class ClienteChat {
    public:
        ClienteChat();
        //ClienteChat(std::string, std::string);
        ~ClienteChat();

        void start();
        void chat();

    private:
        std::string ip, name, msg, quit;

        boost::asio::io_service io_service;
        tcp::resolver * resolver; //el resolver se encarga de transformar los strings en ips y puertos
        tcp::socket * socket;
        tcp::resolver::query * listaendpoints;
        tcp::resolver::iterator * endpoints;

};

#endif /* CLIENTECHATH*/