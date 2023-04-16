#include "doorlock.h"
#include <wiringPi.h>
#include <stdexcept>

Doorlock::Doorlock(int pin) : pin(pin) {
    if (wiringPiSetupGpio() == -1) {
        throw std::runtime_error("wiringPi setup failed");
    }
    pinMode(pin, OUTPUT);
}

void Doorlock::run(){
    Doorlock gpio(26);
    gpio.setPinHigh();
    gpio.delayMillis(5000);
    gpio.setPinLow();
    gpio.delayMillis(1000);
}

void Doorlock::setPinHigh() {
    digitalWrite(pin, HIGH);
}

void Doorlock::setPinLow() {
    digitalWrite(pin, LOW);
}

void Doorlock::delayMillis(int millis) {
    delay(millis);
}
