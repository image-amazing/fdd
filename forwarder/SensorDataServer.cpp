#include"SensorDataServer.h"

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

SensorDataServer::serve(){
    while(true){
        TCPSocket connSock;
        SocketAddress clientAddr;
        sock_.accept(connSock,clientAddr);
        SocketBuffer sockBuf;
        connSock.receive(sockBuf);
        cout<<static_cast<char *>(sockBuf.getBuffer())<<endl;
    }
}
