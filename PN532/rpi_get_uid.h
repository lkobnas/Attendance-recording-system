#include <iostream>
#include <string>
#include <iomanip>
#include <cstdint>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <QObject>
#include "pn532.h"
#include "pn532_rpi.h"

class RFID : public QObject{
    public:
        static std::string get_uid(void);

};
