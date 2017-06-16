#pragma once
#ifndef SERVERCHATH
#define SERVERCHATH
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <boost/asio.hpp>
#include <boost/array.hpp> //Boost::array

using boost::asio::ip::tcp;

class ServerChat {
    public:
        ServerChat();
        ~ServerChat();

        void listen();
        void read();

    private:
        std::vector<std::string> mensajesLog;
        std::vector<tcp::socket> sockets;

        boost::array<char,128> msg;
        boost::asio::io_service io_service; //io_service

        tcp::acceptor * acceptor;
        tcp::socket * socket;
        boost::system::error_code error;
};

#endif