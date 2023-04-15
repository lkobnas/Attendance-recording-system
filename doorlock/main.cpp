#include <iostream>
#include "GpioController.h"

int main() {
    try {
        GpioController gpio(26);

        gpio.setPinHigh();
        gpio.delayMillis(5000);

        gpio.setPinLow();
        gpio.delayMillis(1000);
    }
    catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
