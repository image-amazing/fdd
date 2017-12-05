#include"Exception.h"

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
    return ("socket exception: "+exceptionInfo_).c_str();
}
