#include <iostream>
#include "doorlock.h"

int main() {
    try {
        Doorlock gpio(26);
        gpio.run();
    }
    catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
