#include"ResultProcessor.h"

int main(){
    ResultProcessor rp(1234);
    rp.set_fddDataHome("/home/lyfe667/fdd_data/");
    rp.set_eeFolder("eyeEvidence/");
    rp.set_meFolder("mouthEvidence/");
    rp.set_resultFolder("result");
    rp.set_tarFolder("/home/lyfe667/fdd_data/tar");
    rp.run();
    return 0;
}
