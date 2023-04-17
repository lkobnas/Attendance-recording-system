#ifndef DOORLOCK_H
#define DOORLOCK_H

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
