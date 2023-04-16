#include "fingerprint.h"
#include <iostream>
#include <wiringPi.h>
int main(){
    Fingerprint fp;
    int result = fp.fp_init();
    // fp.syncConfig();
    std::cout<< "result:" << result;
    // wiringPiSetup();
    // pinMode(1, INPUT);
    //fp.fp_list();
    //int fpID = fp.fp_add();
    while(true){
        int fpID = fp.fp_identify();
        std::cout<<fpID;
    }

}