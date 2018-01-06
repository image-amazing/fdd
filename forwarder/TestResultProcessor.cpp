#include"MessageQueue.h"
#include<MessageQueue.h>

using namespace forwarder;

int main(){
    MessageQueue msgQue(1234);
    FatigueMessage fmsg;
    fmsg.setResultFileName("1_2017-11-26_21.42.23_FATIGUE.txt");
    fmsg.setEvidenceName("1_2017-11-26_22.1.12_FATIGUE.avi");
    msgQue.push<FatigueMessage>(fmsg,4);
    return 0;
}
