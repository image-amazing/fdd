#include"SensorDataServer.h"
#include<iostream>

namespace forwarder{
SensorDataServer::SensorDataServer(const SocketAddress &sockAddr)
    :sock_(sockAddr.getFamily()),sockAddr_(sockAddr)
{
    sock_.bind(sockAddr_);
    sock_.listen();
}

SensorDataServer::~SensorDataServer(){
    try{
        sock_.close();
    }catch(...){

    }
}

void SensorDataServer::serve(){
    while(true){
        TCPSocket connSock;
        SocketAddress clientAddr;
        sock_.accept(connSock,clientAddr);
        SocketBuffer sockBuf(1024);
        connSock.receive(sockBuf);
        std::cout<<static_cast<char *>(sockBuf.getBuffer())<<std::endl;
    }
}
}
