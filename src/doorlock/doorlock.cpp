#include "doorlock.h"
#include <wiringPi.h>
#include <stdexcept>


/// @brief Initialise WiringPi library and GPIO pin mode
/// @param p 
void Doorlock::init(int p){
    pin = p;
    if (wiringPiSetupGpio() == -1) {
        throw std::runtime_error("wiringPi setup failed");
    }
    pinMode(pin, OUTPUT);
}
/// @brief Run the doorlock opening sequence
void Doorlock::run(){
    setPinHigh();
    delayMillis(1500);
    setPinLow();
    delayMillis(1000);
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
