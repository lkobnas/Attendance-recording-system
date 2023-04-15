#include "fingerprint.h"
#include <iostream>
int main(){
    Fingerprint fp;
    int result = fp.fp_init();
    std:cout<< "result:" << result;
    fp.fp_add(9);
}