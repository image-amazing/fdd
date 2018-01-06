#ifndef RESULTPROCESSOR_H
#define RESULTPROCESSOR_H
#include<MessageQueue.h>

class ResultProcessor{
private:
    forwarder::MessageQueue msgQue_;
    std::string fddDataHome_;
    std::string eeFolder_;
    std::string meFolder_;
    std::string resultFolder_;
    std::string logFolder_;
    std::string tarFolder_;
    std::string wavHome_;
    std::string distractionAudio_;
    std::string yawnAudio_;
    std::string frequentYawnAudio_;
    std::string sleepyAudio_;
public:
    ResultProcessor(key_t msgKey);
    ~ResultProcessor();
public:
    void initProcessor();
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
    void set_logFolder(const std::string &logFolder){
        logFolder_=logFolder;
    }
    void set_tarFolder(const std::string &tarFolder){
        tarFolder_=tarFolder;
    }
    void set_wavHome(const std::string &wavHome){
        wavHome_=wavHome;
    }
    void set_distracionAudio(const std::string &distrationAudio){
        distractionAudio_=distrationAudio;
    }
    void set_yawnAudio(const std::string &yawnAudio){
        yawnAudio_=yawnAudio;
    }
    void set_frequentYawnAudio(const std::string &frequentYawnAudio){
        frequentYawnAudio_=frequentYawnAudio;
    }
    void set_sleepyAudio(const std::string &sleepyAudio){
        sleepyAudio_=sleepyAudio;
    }
private:
    void process(const  forwarder::FatigueMessage &fmsg,int msgType);
};
#endif // RESULTPROCESSOR_H
