#ifndef GPIOCONTROLLER_H
#define GPIOCONTROLLER_H

class GpioController {
public:
    int pin;
    GpioController(int pin);
    void setPinHigh();
    void setPinLow();
    void delayMillis(int millis);

};

#endif // GPIOCONTROLLER_H
