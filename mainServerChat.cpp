#include <iostream>
#include "ServerChat.h"


int main(int argc,char * argv[]){

    ServerChat * server = new ServerChat();
    server->listen();
    return 0;
}