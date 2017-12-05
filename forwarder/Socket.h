#ifndef SOCKET_H
#define SOCKET_H
#include<sys/socket.h>
#include<string>
#include"Exception.h"

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
   virtual int send(const void *buf,size_t len,int flags=0)=0;
   virtual int receive(void *buf,size_t len,int flags=0)=0;
};

#endif // SOCKET_H
