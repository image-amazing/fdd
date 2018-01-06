#include"ResultProcessor.h"
#include<assert.h>
#include<libconfig.h++>
#include<unistd.h>
#include<assert.h>
#include<stdio.h>
#include<iostream>

using namespace forwarder;

struct ResultMessageProcessorConfigure{
     std::string outputDataHome;
     std::string eyeEvidenceFolder;
     std::string mouthEvidenceFolder;
     std::string resultFolder;
     std::string logFolder;
     std::string tarFolder;
     std::string resultMessageQueueKey;
     std::string wavHome;
     std::string distractionAudio;
     std::string yawnAudio;
     std::string frequentYawnAudio;
     std::string sleepyAudio;
     void configFromFile(const std::string &configFile){
         libconfig::Config cfg;
         cfg.readFile(configFile.c_str());
         outputDataHome=cfg.lookup("outputDataHome").c_str();
         eyeEvidenceFolder=cfg.lookup("eyeEvidenceFolder").c_str();
         mouthEvidenceFolder=cfg.lookup("mouthEvidenceFolder").c_str();
         resultFolder=cfg.lookup("resultFolder").c_str();
         logFolder=cfg.lookup("logFolder").c_str();
         tarFolder=cfg.lookup("tarFolder").c_str();
         resultMessageQueueKey=cfg.lookup("resultMessageQueueKey").c_str();
         wavHome=cfg.lookup("wavHome").c_str();
         distractionAudio=cfg.lookup("distractionAudio").c_str();
         yawnAudio=cfg.lookup("yawnAudio").c_str();
         frequentYawnAudio=cfg.lookup("frequentYawnAudio").c_str();
         sleepyAudio=cfg.lookup("sleepyAudio").c_str();
     }
};

int main(int argc,char *args[]){
    assert(2==argc);
    std::string configFile=args[1];
    ResultMessageProcessorConfigure rmpc;
    rmpc.configFromFile(configFile);
    if(0==access(rmpc.tarFolder.c_str(),F_OK)){
      std::cout<<"good"<<std::endl;
    }else{
       std::cout<<rmpc.tarFolder<<" dosen\'t exist"<<std::endl;
       std::cout<<"bad"<<std::endl;
    }
     ResultProcessor rp(atoi(rmpc.resultMessageQueueKey.c_str()));
    rp.set_fddDataHome(rmpc.outputDataHome);
    rp.set_eeFolder(rmpc.eyeEvidenceFolder);
    rp.set_meFolder(rmpc.mouthEvidenceFolder);
    rp.set_resultFolder(rmpc.resultFolder);
    rp.set_logFolder(rmpc.logFolder);
    rp.set_tarFolder(rmpc.tarFolder);
    rp.set_wavHome(rmpc.wavHome);
    rp.set_distracionAudio(rmpc.distractionAudio);
    rp.set_yawnAudio(rmpc.yawnAudio);
    rp.set_frequentYawnAudio(rmpc.frequentYawnAudio);
    rp.set_sleepyAudio(rmpc.sleepyAudio);

    rp.initProcessor();

    rp.run();
    return 0;
}
