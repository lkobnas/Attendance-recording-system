#include "fingerprint.h"
#include <iostream>
#include <wiringPi.h>
int main(){
    extern AS608 g_as608;
    extern int g_fd;
    extern int g_verbose;
    extern char  g_error_desc[128];
    extern uchar g_error_code;

    Fingerprint fp;
    int result = fp.fp_init();
    std::cout<< "result:" << result;
    // wiringPiSetup();
    // pinMode(1, INPUT);
    fp.fp_list();
    //fp.fp_add(9);
}