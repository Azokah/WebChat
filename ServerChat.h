#pragma once
#ifndef SERVERCHATH
#define SERVERCHATH
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <boost/asio.hpp>
#include <boost/array.hpp> //Boost::array
#include <boost/bind.hpp>

using boost::asio::ip::tcp;

class ServerChat
{
  public:
    ServerChat(boost::asio::io_service &);
    ~ServerChat();

    void listen();
    void read();

    void manejarLectura();
    void manejarEscritura();
  private:
    std::vector<std::string> mensajesLog;
    std::vector<tcp::socket> sockets;

    boost::array<char, 128> msg;
    boost::asio::io_service &io_service; //io_service

    tcp::resolver * resolver;
    tcp::acceptor *acceptor;
    tcp::socket *socket;
    boost::system::error_code error;

    void handlerLectura(const boost::system::error_code &ec,
    std::size_t bytes_transferred){};
    void handlerEscritura(const boost::system::error_code &ec,
    std::size_t bytes_transferred){};

    static void handlerAccept(const boost::system::error_code& error){
        if (!error){
            // Accept succeeded.
            std::cout<<"Se ha unido un nuevo usuario."<<std::endl;
        }
    };
};

#endif