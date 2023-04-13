#include "rpi_get_uid.h"
#include <string>
#include <iostream>

using namespace std;
int main(int argc, char** argv) {
    RFID rfid;
    string uid;
    uid = rfid.get_uid();
    cout<<"\r\nThe string type of uid is: "<<uid<<"\r\n";
return 0;
}