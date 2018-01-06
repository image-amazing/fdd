#include"ResultProcessor.h"
#include<iostream>
#include<sstream>
#include<glog/logging.h>

using namespace forwarder;

ResultProcessor::ResultProcessor(key_t msgKey)
    :msgQue_(msgKey),fddDataHome_("./"),eeFolder_("eyeEvidence")
    ,meFolder_("mouthEvidence"),resultFolder_("result"),logFolder_("./")
    ,wavHome_("./"),distractionAudio_("distraciont.wav"),yawnAudio_("yawn.wav")
    ,frequentYawnAudio_("frequentyawn.wav"),sleepyAudio_("sleepy.wav")
{

}

ResultProcessor::~ResultProcessor(){
    msgQue_.deleteQueue();
    google::ShutdownGoogleLogging();
}

void ResultProcessor::initProcessor(){
    google::InitGoogleLogging("ResultProcessor");
    google::SetLogDestination(google::GLOG_INFO,(fddDataHome_+"/"+logFolder_+"/rp").c_str());
}

void ResultProcessor::run(){
    while(true){
        Message<FatigueMessage> msg;
        msgQue_.pop<FatigueMessage>(msg);
        /*std::cout<<msg.getData()->getResultFileName()<<std::endl;
        std::cout<<msg.getData()->getEvidenceName()<<std::endl;
        std::cout<<msg.getType()<<std::endl;*/
        process(*(msg.getData()),msg.getType());
    }
}

void ResultProcessor::process(const FatigueMessage &fmsg,int msgType){
    std::string resultFileName=fmsg.getResultFileName();
    std::string fileTitle=resultFileName.substr(0,resultFileName.find_last_of("."));
    switch(msgType){
    case 0:
    {
    }
        break;
    case 1:
    {
       LOG(INFO)<<"distraction";
        std::stringstream sout;
        std::string cmd="";
#ifdef WITH_SOUND
         sout<<"play "<<wavHome_+distractionAudio_<<" 2>&1 1>>play.log &";
         cmd=sout.str();
         system(cmd.c_str());
         LOG(INFO)<<cmd<<" :executed";
#endif
        sout.str("");
        sout<<"tar -cf "<<tarFolder_<<"/"<<fileTitle<<".tar "<<" -C "
           <<fddDataHome_<<"/"<<resultFolder_<<" "<<fmsg.getResultFileName();
         cmd=sout.str();
         system(cmd.c_str());
         LOG(INFO)<<cmd<<" :executed";
         sout.str("");
         sout<<"rm "<<fddDataHome_<<"/"<<resultFolder_<<"/"<<fmsg.getResultFileName()<<" &";
         cmd=sout.str();
         system(cmd.c_str());
         LOG(INFO)<<cmd<<" :executed";
         google::FlushLogFiles(google::INFO);
    }
        break;
    case 2:
    {
        LOG(INFO)<<"yawn";
        std::stringstream sout;
         std::string cmd="";
#ifdef WITH_SOUND
         sout<<"play "<<wavHome_+ yawnAudio_<<"  2>&1 1>play.log &";
         cmd = sout.str();
         system(cmd.c_str());
          LOG(INFO)<<cmd<<" :executed";
#endif
        sout.str("");
        sout<<"tar -cf "<<tarFolder_<<"/"<<fileTitle<<".tar "<<" -C "
           <<fddDataHome_<<"/"<<resultFolder_<<" "<<fmsg.getResultFileName();
         cmd=sout.str();
         system(cmd.c_str());
         LOG(INFO)<<cmd<<" :executed";
          sout.str("");
          sout<<"rm "<<fddDataHome_<<"/"<<resultFolder_<<"/"<<fmsg.getResultFileName()<<" &";
          cmd=sout.str();
          system(cmd.c_str());
          LOG(INFO)<<cmd<<" :executed";
         google::FlushLogFiles(google::INFO);
    }
        break;
    case 3:
    {
        LOG(INFO)<<"frequent yawn";
        std::stringstream sout;
        std::string cmd="";
#ifdef WITH_SOUND
        sout<<"play "<<wavHome_+ frequentYawnAudio_<<" 2>&1 1>play.log &";
        cmd = sout.str();
        system(cmd.c_str());
         LOG(INFO)<<cmd<<" :executed";
#endif
         sout.str("");
        sout<<"tar -cf "<<tarFolder_<<"/"<<fileTitle<<".tar "<<
              " -C "<<fddDataHome_<<"/"<<resultFolder_<<" "<<fmsg.getResultFileName()<<
              " -C "<<fddDataHome_<<"/"<<meFolder_<<" "<<fmsg.getEvidenceName();
        cmd=sout.str();
        system(cmd.c_str());
        LOG(INFO)<<cmd<<" :executed";
        sout.str("");
        sout<<"rm "<<fddDataHome_<<"/"<<resultFolder_<<"/"<<fmsg.getResultFileName()
           <<" "<<fddDataHome_<<"/"<<meFolder_<<"/"<<fmsg.getEvidenceName()<<" &";
        cmd=sout.str();
        system(cmd.c_str());
        LOG(INFO)<<cmd<<" :executed";
        google::FlushLogFiles(google::INFO);
    }
        break;
    case 4:
    {
        LOG(INFO)<<"sleepy";
        std::stringstream sout;
        std::string cmd="";
#ifdef WITH_SOUND
        sout<<"play "<<wavHome_+ sleepyAudio_<<" 2>&1 1>play.log &";
        cmd = sout.str();
        system(cmd.c_str());
         LOG(INFO)<<cmd<<" :executed";
#endif
        sout.str("");
        sout<<"tar -cf "<<tarFolder_<<"/"<<fileTitle<<".tar "<<
              " -C "<<fddDataHome_<<"/"<<resultFolder_<<" "<<fmsg.getResultFileName()<<
              " -C "<<fddDataHome_<<"/"<<eeFolder_<<" "<<fmsg.getEvidenceName();
        cmd=sout.str();
        system(cmd.c_str());
         LOG(INFO)<<cmd<<" :executed";
         sout.str("");
         sout<<"rm "<<fddDataHome_<<"/"<<resultFolder_<<"/"<<fmsg.getResultFileName()
            <<" "<<fddDataHome_<<"/"<<eeFolder_<<"/"<<fmsg.getEvidenceName()<<" &";
         cmd=sout.str();
         system(cmd.c_str());
         LOG(INFO)<<cmd<<" :executed";
         google::FlushLogFiles(google::INFO);
    }
        break;
    }
}


