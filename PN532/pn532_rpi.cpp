/**************************************************************************
 *  @file     pn532_rpi.c
 *  @author   Yehui from Waveshare
 *  @license  BSD
 *  
 *  This implements the peripheral interfaces.
 *  
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documnetation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to  whom the Software is
 * furished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 **************************************************************************/

#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <time.h>

#include "wiringPi.h"
#include "wiringPiSPI.h"
#include "wiringSerial.h"
#include "pn532_rpi.h"

#define _RESET_PIN                      (20)
#define _REQ_PIN                        (16)

#define _SPI_STATREAD                   (0x02)
#define _SPI_DATAWRITE                  (0x01)
#define _SPI_DATAREAD                   (0x03)
#define _SPI_READY                      (0x01)
#define _SPI_CHANNEL                    (0)
#define _NSS_PIN                        (4)

#define _I2C_READY                      (0x01)
#define _I2C_ADDRESS                    (0x48 >> 1)
#define _I2C_CHANNEL                    (1)

static int fd = 0;
using namespace std;

/**************************************************************************
 * Reset and Log implements
 **************************************************************************/
int pn532_rpi::PN532_Reset(void) {
    digitalWrite(_RESET_PIN, HIGH);
    delay(100);
    digitalWrite(_RESET_PIN, LOW);
    delay(500);
    digitalWrite(_RESET_PIN, HIGH);
    delay(100);
    return PN532_STATUS_OK;
}

void pn532_rpi::PN532_Log(const char* log) {
    printf(log);
    printf("\r\n");
}
/**************************************************************************
 * End: Reset and Log implements
 **************************************************************************/

/**************************************************************************
 * I2C
 **************************************************************************/
int pn532_rpi::PN532_I2C_ReadData(uint8_t* data, uint16_t count) {
    uint8_t status[] = {0x00};
    uint8_t frame[count + 1];
    read(fd, status, sizeof(status));
    if (status[0] != _I2C_READY) {
        return PN532_STATUS_ERROR;
    }
    read(fd, frame, count + 1);
    for (uint8_t i = 0; i < count; i++) {
        data[i] = frame[i + 1];
    }
    return PN532_STATUS_OK;
}

int pn532_rpi::PN532_I2C_WriteData(uint8_t *data, uint16_t count) {
    write(fd, data, count);
    return PN532_STATUS_OK;
}

bool pn532_rpi::PN532_I2C_WaitReady(uint32_t timeout) {
    uint8_t status[] = {0x00};
    struct timespec timenow;
    struct timespec timestart;
    clock_gettime(CLOCK_MONOTONIC, &timestart);
    while (1) {
        read(fd, status, sizeof(status));
        if (status[0] == _I2C_READY) {
            return true;
        } else {
            delay(5);
        }
        clock_gettime(CLOCK_MONOTONIC, &timenow);
        if ((timenow.tv_sec - timestart.tv_sec) * 1000 + \
            (timenow.tv_nsec - timestart.tv_nsec) / 1000000 > timeout) {
            break;
        }
    }
    // Time out!
    return false;
}

int pn532_rpi::PN532_I2C_Wakeup(void) {
    digitalWrite(_REQ_PIN, HIGH);
    delay(100);
    digitalWrite(_REQ_PIN, LOW);
    delay(100);
    digitalWrite(_REQ_PIN, HIGH);
    delay(500);
    return PN532_STATUS_OK;
}

void pn532_rpi::PN532_I2C_Init(PN532* pn532) {
    // init the pn532 functions
    pn532->reset = PN532_Reset;
    pn532->read_data = PN532_I2C_ReadData;
    pn532->write_data = PN532_I2C_WriteData;
    pn532->wait_ready = PN532_I2C_WaitReady;
    pn532->wakeup = PN532_I2C_Wakeup;
    pn532->log = PN532_Log;
    char devname[20];
    snprintf(devname, 19, "/dev/i2c-%d", _I2C_CHANNEL);
    fd = open(devname, O_RDWR);
    if (fd < 0) {
        fprintf(stderr, "Unable to open i2c device: %s\n", strerror(errno));
        return;
    }
    if (ioctl(fd, I2C_SLAVE, _I2C_ADDRESS) < 0) {
        fprintf(stderr, "Unable to open i2c device: %s\n", strerror(errno));
        return;
    }
    if (wiringPiSetupGpio() < 0) {  // using Broadcom GPIO pin mapping
        return;
    }
    pinMode(_REQ_PIN, OUTPUT);
    pinMode(_RESET_PIN, OUTPUT);
    // hardware reset
    pn532->reset();
    // hardware wakeup
    pn532->wakeup();
}
/**************************************************************************
 * End: I2C
 **************************************************************************/
