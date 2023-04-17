#ifndef DOORLOCK_H
#define DOORLOCK_H

/// @brief A class for simulating a doorlock mechanism by a LED using GPIO in raspberry pi 
class Doorlock {
public:
    void init(int pin);
    void run();
private:
    int pin;
    void setPinHigh();
    void setPinLow();
    void delayMillis(int millis);

};

#endif // DOORLOCK_H
