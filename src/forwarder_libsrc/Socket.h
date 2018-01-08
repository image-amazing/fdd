#ifndef SOCKET_H
#define SOCKET_H

#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string>
#include"Exception.h"

namespace forwarder{
class SocketBuffer{
private:
    int maxSize_;
    int size_;
    void *pBuf_;
public:
    SocketBuffer(int maxSize);
    ~SocketBuffer();
public:
    void copyFrom(void *pStart,int len);
public:
    const int getMaxSize()const;
    const int getSize()const;
    void setSize(int size);
    void * getBuffer();
    void * getBuffer() const;
};

class SocketAddress{
public:
    static const unsigned short defaultPort;
private:
        sockaddr_in sa_;
public:
         SocketAddress();
         SocketAddress(unsigned short port,const std::string addr="",int family=AF_INET);
         SocketAddress(const sockaddr_in &sa);
         ~SocketAddress();
public:
        SocketAddress & operator=(const sockaddr_in &sa);
        operator sockaddr_in() const;
        void setFamily(int family);
       const int getFamily() const;
        void setPort(unsigned short port);
        const unsigned short getPort() const;
        void setAddr(const std::string &addr="");
        const std::string getAddr() const;
};

class Socket{
protected:
    int sockfd_;
    int type_;
    int family_;
    int protocol_;
public:
    Socket(int type,int family=AF_INET,int protocol=0);
    virtual ~Socket();
public:
   void bind(unsigned short port,const std::string &addr="");
   void bind(const SocketAddress &sa);
public:
    void setSockfd(int sockfd);
    int getSockfd();
    void setType(int type);
    int getType();
    void setFamily(int family);
    int getFamily();
    void setProtocol(int protocol);
    int getProtocol();
};

}
#endif // SOCKET_H
