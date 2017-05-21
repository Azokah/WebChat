#pragma once
#include <iostream>
#include <boost/asio.hpp>
#include <utility>
#include "Participante.h"
#include "Sala.h"

using boost::asio::ip::tcp;

class Sesion : public Participante{
    public:
        Sesion(tcp::socket, Sala *);
        ~Sesion();

        void iniciar();
        void entregar(MensajeChat*);


    private:
        tcp::socket socket;
        Sala * sala;

        std::deque<MensajeChat*> mensajesEscritos;
        std::deque<MensajeChat*> mensajesRecibidos;

        void escribir();
};

Sesion::Sesion(tcp::socket S, Sala * s): socket(std::move(S)), sala(s){};
Sesion::~Sesion(){};

void Sesion::iniciar(){
    sala->unir(this);  
};

void Sesion::entregar(MensajeChat* msg){
    bool escribiendo = !mensajesEscritos.empty();
    mensajesEscritos.push_back(msg);
    if(!escribiendo){
        escribir();
    }
};

void Sesion::escribir(){
    auto self(this);
    boost::asio::async_write(socket,boost::asio::buffer(mensajesEscritos.front()->data(),mensajesEscritos.front()->lenght()),
    [this,self](boost::system::error_code ec, std::size_t /*length*/){
        if(!ec){
            mensajesEscritos.pop_front();
            if(!mensajesEscritos.empty()){
                escribir();
            }
        } else {
            sala.remover(this);
        }
    });
};

