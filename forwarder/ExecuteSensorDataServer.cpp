#include"SensorDataServer.h"

using namespace forwarder;

const unsigned short servicePort=2018;
const std::string serviceIP="127.0.0.1";

int main(){
    SocketAddress sockAddr(servicePort,serviceIP);
    SensorDataServer  sds(sockAddr);
    sds.serve();
    return 0;
}
