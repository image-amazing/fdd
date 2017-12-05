#include"Socket.h"
#include<unistd.h>
#include<string.h>
#include<netinet/in.h>
#include<arpa/inet.h>

Socket::Socket(int type,int family,int protocol)
    :type_(type),family_(family),protocol_(protocol)
{
    sockfd_=::socket(family_,type_,protocol_);
    CHECK<SocketException>(sockfd_!=-1,"fail to create!");
}

Socket::~Socket(){
    close(sockfd_);
}

void Socket::bind(unsigned short port,const std::string &addr){
    struct sockaddr_in sockAddr;
    bzero(static_cast<void *>(&sockAddr),sizeof(sockAddr));
    sockAddr.sin_family=family_;
    sockAddr.sin_port=htonl(port);
    sockAddr.sin_addr.s_addr=(addr.length()==0?htonl(INADDR_ANY):inet_addr(addr.c_str()));
    CHECK<SocketException>(-1!=::bind(sockfd_,(struct sockaddr *)&sockAddr,sizeof(sockAddr)),"fail to bind!");
}
