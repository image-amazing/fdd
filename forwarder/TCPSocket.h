#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include"Socket.h"

class TCPSocket: public Socket{
public:
    TCPSocket(int family=AF_INET,int protocol=0);
    ~TCPSocket();
public:
    void listen(int maxConnNum=5);

};



#endif // TCPSOCKET_H
