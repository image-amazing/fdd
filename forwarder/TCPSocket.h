#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include"Socket.h"

namespace forwarder{

class TCPSocket: public Socket{
public:
    //default constructor
    TCPSocket(int family=AF_INET,int protocol=0);
    ~TCPSocket();
public:
    void connect(const SocketAddress &serverAddr);
    void connect(unsigned short port,const std::string &addr);
    void listen(int maxConnNum=5);
    void accept(TCPSocket &connectedSocket,SocketAddress &clientAddr);
    int send(const void *buf,size_t len,int flags=0);
    int send(const SocketBuffer &sockBuf,int flags=0);
    int receive(void *buf,size_t max_len,int flags=0);
    void receive(SocketBuffer &sockBuf,int flags=0);
    void close();
};

}
#endif // TCPSOCKET_H
