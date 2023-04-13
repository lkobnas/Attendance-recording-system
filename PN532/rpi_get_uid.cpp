#include "rpi_get_uid.h"
using namespace std;


string RFID::get_uid(void) {
    uint8_t buff[255];
    uint8_t uid[MIFARE_UID_MAX_LENGTH];
    int32_t uid_len = 0;
    string uid_str;
    cout<<"Hello!\r\n";
    PN532 pn532; // creat a module object pn532
    pn532_rpi I2C_pn532; // creat a I2C object for raspberry pi
    I2C_pn532.PN532_I2C_Init(&pn532);
    if (PN532_GetFirmwareVersion(&pn532, buff) == PN532_STATUS_OK) {
        //printf("Found PN532 with firmware version: %d.%d\r\n", buff[1], buff[2]);
        cout<<"Found PN532 with firmware version: "<<static_cast<int>(buff[1])<<"."<<static_cast<int>(buff[2])<<"\r\n";
    } else {
        return 0;
    }
    PN532_SamConfiguration(&pn532);
    cout<<"Waiting for RFID/NFC card...\r\n";
    while (1)
    {
        // Check if a card is available to read
        uid_len = PN532_ReadPassiveTarget(&pn532, uid, PN532_MIFARE_ISO14443A, 1000);
        if (uid_len == PN532_STATUS_ERROR) {
            cout<<".";
            fflush(stdout);
        } else {
            cout<<"\r\nFound card with UID: ";
            for (uint8_t i = 0; i < uid_len; i++) {
                //printf("%02x ", uid[i]);
                cout<<hex<<static_cast<int>(uid[i])<<" ";
                uid_str += to_string(uid[i]);
            }
            cout<<"\r\n"<<uid_str;
            break;
        }
    }
return uid_str;
}

