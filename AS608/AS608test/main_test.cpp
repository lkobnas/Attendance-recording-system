#include "fingerprint.h"
#include <iostream>
#include <wiringPi.h>
int main(){
    Fingerprint fp;
    int result = fp.fp_init();
    std::cout<< "result:" << result;
    wiringPiSetup();
    pinMode(1, INPUT);
    fp.fp_add(9);
}