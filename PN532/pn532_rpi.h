

#ifndef PN532_RPI
#define PN532_RPI

#include "pn532.h"



class pn532_rpi{

    public:
        static int PN532_Reset(void);
        static void PN532_Log(const char* log);
        static void PN532_I2C_Init(PN532* dev);
        static int PN532_I2C_ReadData(uint8_t* data, uint16_t count);
        static int PN532_I2C_WriteData(uint8_t *data, uint16_t count);
        static bool PN532_I2C_WaitReady(uint32_t timeout);
        static int PN532_I2C_Wakeup(void);
};


#endif  /* PN532_RPI */
