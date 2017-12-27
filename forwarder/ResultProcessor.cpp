#include"ResultProcessor.h"
#include<iostream>
#include<sstream>

namespace forwarder{

ResultProcessor::ResultProcessor(key_t msgKey)
    :msgQue_(msgKey),fddDataHome_("./"),eeFolder_("eyeEvidence")
    ,meFolder_("mouthEvidence"),resultFolder_("result")
{

}

ResultProcessor::~ResultProcessor(){
    msgQue_.deleteQueue();
}

void ResultProcessor::run(){
    using namespace fdd;
    while(true){
        Message<FatigueMessage> msg;
        msgQue_.pop<FatigueMessage>(msg);
        /*std::cout<<msg.getData()->getResultFileName()<<std::endl;
        std::cout<<msg.getData()->getEvidenceName()<<std::endl;
        std::cout<<msg.getType()<<std::endl;*/
        process(*(msg.getData()),msg.getType());
    }
}

void ResultProcessor::process(const fdd::FatigueMessage &fmsg,int msgType){
    std::string resultFileName=fmsg.getResultFileName();
    std::string fileTitle=resultFileName.substr(0,resultFileName.find_last_of("."));
    switch(msgType){
    case 0:
    {
    }
        break;
    case 1:
    {
        std::stringstream sout;
        sout<<"tar -cf "<<tarFolder_<<"/"<<fileTitle<<".tar "<<" -C "
           <<fddDataHome_<<"/"<<resultFolder_<<" "<<fmsg.getResultFileName();
         std::string cmd=sout.str();
         system(cmd.c_str());
         std::cout<<cmd<<" :executed"<<std::endl;
    }
        break;
    case 2:
    {
        std::stringstream sout;
        sout<<"tar -cf "<<tarFolder_<<"/"<<fileTitle<<".tar "<<" -C "
           <<fddDataHome_<<"/"<<resultFolder_<<" "<<fmsg.getResultFileName();
         std::string cmd=sout.str();
         system(cmd.c_str());
         std::cout<<cmd<<" :executed"<<std::endl;
    }
        break;
    case 3:
    {
        std::stringstream sout;
        sout<<"tar -cf "<<tarFolder_<<"/"<<fileTitle<<".tar "<<
              " -C "<<fddDataHome_<<"/"<<resultFolder_<<" "<<fmsg.getResultFileName()<<
              " -C "<<fddDataHome_<<"/"<<meFolder_<<" "<<fmsg.getEvidenceName();
        std::string cmd=sout.str();
        system(cmd.c_str());
        std::cout<<cmd<<" :executed"<<std::endl;
    }
        break;
    case 4:
    {
        std::stringstream sout;
        sout<<"tar -cf "<<tarFolder_<<"/"<<fileTitle<<".tar "<<
              " -C "<<fddDataHome_<<"/"<<resultFolder_<<" "<<fmsg.getResultFileName()<<
              " -C "<<fddDataHome_<<"/"<<eeFolder_<<" "<<fmsg.getEvidenceName();
        std::string cmd=sout.str();
        system(cmd.c_str());
        std::cout<<cmd<<" :executed"<<std::endl;
    }
        break;
    }
}

}

