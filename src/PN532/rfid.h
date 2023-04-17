#include <iostream>
#include <string>
#include <iomanip>
#include <cstdint>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <QObject>
#include "PN532_lib/pn532.h"
#include "PN532_lib/pn532_rpi.h"

/// @brief RFID Class. Inherits from QObject 
class RFID : public QObject{
    public:
        static std::string get_uid(void);

};
