#pragma once
#include <iostream>
#include "MensajeChat.h"

//Participante-abstracta
class Participante
{
public:
  virtual ~Participante() {}
  virtual void entregar(MensajeChat * msg) = 0;
};
