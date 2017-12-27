#include"Forwarder.h"

using namespace forwarder;

const int msgKey=2017;
const unsigned short destPort=2018;
const std::string destIP="127.0.0.1";

int main(){
    SocketAddress destAddr(destPort,destIP);
    Forwarder forwarder(static_cast<key_t>(msgKey),destAddr);
    forwarder.forward();
    return 0;
}
