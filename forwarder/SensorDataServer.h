#ifndef SENSORDATASERVER_H
#define SENSORDATASERVER_H
#include"TCPSocket.h"

namespace forwarder{

class SensorDataServer{
private:
       TCPSocket sock_;
       SocketAddress sockAddr_;
public:
       SensorDataServer(const SocketAddress &sockAddr);
       ~SensorDataServer();
public:
       void serve();
};

}
#endif // SENSORDATASERVER_H
