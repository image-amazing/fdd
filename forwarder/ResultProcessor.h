#ifndef RESULTPROCESSOR_H
#define RESULTPROCESSOR_H
#include<FDDGlobal.h>
#include"MessageQueue.h"

namespace forwarder{
class ResultProcessor{
private:
    MessageQueue msgQue_;
    std::string fddDataHome_;
    std::string eeFolder_;
    std::string meFolder_;
    std::string resultFolder_;
    std::string tarFolder_;
public:
    ResultProcessor(key_t msgKey);
    ~ResultProcessor();
public:
    void run();
public:
    void set_fddDataHome(const std::string &fddDataHome){
        fddDataHome_=fddDataHome;
    }
    void set_eeFolder(const std::string &eeFolder){
        eeFolder_=eeFolder;
    }
    void set_meFolder(const std::string &meFolder){
        meFolder_=meFolder;
    }
    void set_resultFolder(const std::string &resultFolder){
        resultFolder_=resultFolder;
    }
    void set_tarFolder(const std::string &tarFolder){
        tarFolder_=tarFolder;
    }
private:
    void process(const fdd::FatigueMessage &fmsg,int msgType);
};
}
#endif // RESULTPROCESSOR_H
