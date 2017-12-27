#include"ResultProcessor.h"
#include<assert.h>
#include<libconfig.h++>
#include<unistd.h>
#include<assert.h>
#include<stdio.h>

using namespace forwarder;

int main(int argc,char *args[]){
    assert(2==argc);
    std::string configFile=args[1];
     libconfig::Config cfg;
     cfg.readFile(configFile.c_str());
     std::string outputDataHome=cfg.lookup("outputDataHome").c_str();
     std::string eyeEvidenceFolder=cfg.lookup("eyeEvidenceFolder").c_str();
     std::string mouthEvidenceFolder=cfg.lookup("mouthEvidenceFolder").c_str();
     std::string resultFolder=cfg.lookup("resultFolder").c_str();
     std::string tarFolder=cfg.lookup("tarFolder").c_str();
    if(0==access(tarFolder.c_str(),F_OK)){
      std::cout<<"good"<<std::endl;
    }else{
       std::cout<<tarFolder<<" dosen\'t exist"<<std::endl;
       std::cout<<"bad"<<std::endl;
    }
     std::string resultMessageQueueKey=cfg.lookup("resultMessageQueueKey").c_str();

     ResultProcessor rp(atoi(resultMessageQueueKey.c_str()));
    rp.set_fddDataHome(outputDataHome);
    rp.set_eeFolder(eyeEvidenceFolder);
    rp.set_meFolder(mouthEvidenceFolder);
    rp.set_resultFolder(resultFolder);
    rp.set_tarFolder(tarFolder);
    rp.run();
    return 0;
}
