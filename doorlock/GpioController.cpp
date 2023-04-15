#include "GpioController.h"
#include <wiringPi.h>
#include <stdexcept>

GpioController::GpioController(int pin) : pin(pin) {
    if (wiringPiSetupGpio() == -1) {
        throw std::runtime_error("wiringPi setup failed");
    }

    pinMode(pin, OUTPUT);
}

void GpioController::setPinHigh() {
    digitalWrite(pin, HIGH);
}

void GpioController::setPinLow() {
    digitalWrite(pin, LOW);
}

void GpioController::delayMillis(int millis) {
    delay(millis);
}
