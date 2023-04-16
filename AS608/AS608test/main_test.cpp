#include "fingerprint.h"
#include <iostream>
#include <wiringPi.h>
int main(){

    Fingerprint fp;
    int result = fp.fp_init();
    //std::cout<< "result:" << result<<endl;
    fp.fp_list();
    //int fpID = fp.fp_add();
    //std::cout<< "fpID: "<<fpID;
}