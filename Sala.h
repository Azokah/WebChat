#pragma once
#include <iostream> 
#include <deque> //To store chat messages
#include "Participante.h"
#include "MensajeChat.h"

#define MENSAJES_MAX 100

//Sala de chat
class Sala
{
public:
  Sala();
  ~Sala();

  void unir(Participante*);
  void remover(Participante*);
  void entregar(MensajeChat*);


private:

  std::deque<Participante*> participantes;
  std::deque<MensajeChat*> mensajes;
};

void Sala::unir(Participante* ppte){
  participantes.push_back(ppte);
  for (auto msg : mensajes) ppte->entregar(msg);
};

void Sala::remover(Participante* ppte){
  //for(auto participante : participantes)
    //participantes.erase(ppte);
    std::cout<<"Se hubiera removido: "<<std::endl;
}

void Sala::entregar(MensajeChat* msg){
  mensajes.push_back(msg);
  while(mensajes.size() > MENSAJES_MAX) mensajes.pop_front();

  for(auto participante : participantes) participante->entregar(msg);
}
