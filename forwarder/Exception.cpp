#include"Exception.h"

namespace forwarder{
Exception::Exception(const std::string &exceptionInfo)
    :exceptionInfo_(exceptionInfo)
{

}

Exception::~Exception(){

}

const char * Exception::what() const noexcept{
    return exceptionInfo_.c_str();
}

SocketException::SocketException(const std::string &exceptionInfo)
    :Exception(exceptionInfo)
{

}

const char * SocketException::what() const noexcept{
    return ("SocketException: "+exceptionInfo_).c_str();
}

MessageException::MessageException(const std::string &exceptionInfo)
    :Exception(exceptionInfo)
{

}

const char * MessageException::what()const noexcept{
    return ("MessageException: "+exceptionInfo_).c_str();
}

MessageQueueException::MessageQueueException(const std::string &exceptionInfo)
    :Exception(exceptionInfo)
{

}

const char * MessageQueueException::what()const noexcept{
    return ("MessageQueueException: "+exceptionInfo_).c_str();
}

}















