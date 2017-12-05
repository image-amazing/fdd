#ifndef EXCEPTION_H
#define EXCEPTION_H
#include<exception>
#include<string>

template<typename ExceptionType>
inline void CHECK(bool expression,const std::string &exceptionInfo){
    if(!expression){
            throw ExceptionType(exceptionInfo);
    }
}

class Exception:public std::exception{
protected:
    std::string exceptionInfo_;
public:
    Exception(const std::string &exceptionInfo);
    virtual ~Exception();
public:
    const char * what() const noexcept override;
};

class SocketException:public Exception{
public:
    SocketException(const std::string &exceptionInfo);
public:
    const char * what() const noexcept override;
};

#endif // EXCEPTION_H