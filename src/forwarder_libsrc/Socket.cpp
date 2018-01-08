#include"Socket.h"
#include<string.h>
namespace forwarder{
SocketBuffer::SocketBuffer(int maxSize)
    :maxSize_(maxSize),size_(0),pBuf_(nullptr)
{
    Check<Exception>(maxSize>0,"SocketBuffer size must be greater than 0");
    pBuf_=static_cast<void *>(new char[maxSize_]);
}

SocketBuffer::~SocketBuffer(){
    try{
        delete [] static_cast<char *>(pBuf_);
    }catch(...){

    }
}

void SocketBuffer::copyFrom(void *pStart,int length){
    Check<Exception>(nullptr!=pBuf_,"SocketBuffer pBuf_ can't be nullptr");
    Check<Exception>(nullptr!=pStart,"SocketBuffer copyFrom pStart can't be nullptr");
    Check<Exception>(length<=maxSize_&&length>=0,"SocketBuffer copyFrom len exception");
    memcpy(pBuf_,pStart,length);
    size_=length;
}

const int SocketBuffer::getMaxSize()const{
    return maxSize_;
}

const int SocketBuffer::getSize()const{
  return size_;
}


void SocketBuffer::setSize(int size){
    size_=size;
}

 void *SocketBuffer::getBuffer(){
    return pBuf_;
}

 void *SocketBuffer::getBuffer() const{
    return pBuf_;
}

const unsigned short SocketAddress::defaultPort=2048;
SocketAddress::SocketAddress()
{
    sa_.sin_family=AF_INET;
    sa_.sin_addr.s_addr=htonl(INADDR_ANY);
    sa_.sin_port=htons(defaultPort);
}

SocketAddress::SocketAddress(unsigned short port,const std::string addr,int family){
    sa_.sin_family=family;
    sa_.sin_port=htons(port);
    sa_.sin_addr.s_addr=(addr.length()==0?htonl(INADDR_ANY):inet_addr(addr.c_str()));
}

SocketAddress::SocketAddress(const sockaddr_in &sa)
    :sa_(sa)
{

}

SocketAddress::~SocketAddress(){

}

SocketAddress &SocketAddress::operator=(const sockaddr_in &sa){
    sa_=sa;
}

 SocketAddress::operator sockaddr_in() const{
    return sa_;
}

void SocketAddress::setFamily(int family){
    sa_.sin_family=family;
}

const int SocketAddress::getFamily() const{
     return sa_.sin_family;
}

void SocketAddress::setPort(unsigned short port){
    sa_.sin_port=htons(port);
}

const unsigned short SocketAddress::getPort() const{
    return ntohs(sa_.sin_port);
}

void SocketAddress::setAddr(const std::string &addr){
    sa_.sin_addr.s_addr=(addr.length()==0?htonl(INADDR_ANY):inet_addr(addr.c_str()));
}

const std::string SocketAddress::getAddr() const{
    return inet_ntoa(sa_.sin_addr);
}

Socket::Socket(int type,int family,int protocol)
    :type_(type),family_(family),protocol_(protocol)
{
    sockfd_=::socket(family_,type_,protocol_);
    Check<SocketException>(sockfd_!=-1,"fail to create!");
}

Socket::~Socket(){
    try{
    close(sockfd_);
    }catch(...){

    }
}

void Socket::bind(unsigned short port,const std::string &addr){
    SocketAddress sockAddr(port,addr,family_);
    bind(sockAddr);
}

void Socket::bind(const SocketAddress &sockAddr){
    sockaddr_in sa=static_cast<sockaddr_in>(sockAddr);
    Check<SocketException>(-1!=::bind(sockfd_,(struct sockaddr *)&sa,sizeof(sa)),"fail to bind!");
}

void Socket::setSockfd(int sockfd){
    sockfd_=sockfd;
}

int Socket::getSockfd(){
    return sockfd_;
}

void Socket::setType(int type){
    type_=type;
}

int Socket::getType(){
    return type_;
}

void Socket::setFamily(int family){
    family_=family;
}

int Socket::getFamily(){
    return family_;
}

void Socket::setProtocol(int protocol){
    protocol_=protocol;
}

int Socket::getProtocol(){
    return protocol_;
}
}








