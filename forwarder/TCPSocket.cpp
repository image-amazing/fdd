#include"TCPSocket.h"

TCPSocket::TCPSocket(int family,int protocol)
    :Socket(SOCK_STREAM,family,protocol)
{

}

TCPSocket::~TCPSocket(){

}

TCPSocket::listen(int maxConnNum){
    CHECK<SocketException>(-1!=::listen(sockfd_,maxConnNum),"fail to listen");
}
