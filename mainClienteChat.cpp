#include <iostream>
#include "ClienteChat.h"


int main(int argc,char * argv[]){

    ClienteChat * cliente = new ClienteChat();
    cliente->start();
    return 0;
}