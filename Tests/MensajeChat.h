//
// MensajeChat.hpp
// ~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#pragma once
#include <iostream>
#include <string>

class MensajeChat
{
public:
  MensajeChat(std::string);
  ~MensajeChat();
  
  std::string getData();

  std::string * getDataPtr();

private:
  std::string data;
};

MensajeChat::MensajeChat(std::string mensaje) : data(mensaje){};
MensajeChat::~MensajeChat(){};

std::string MensajeChat::getData(){ return data;};
std::string* MensajeChat::getDataPtr(){ return &data;};