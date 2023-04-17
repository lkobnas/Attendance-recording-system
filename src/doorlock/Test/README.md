# Testing GPIO and doorlock

# Setup
Connect your LED to pin 26 and ground

## Running test

Create and navigate to the build folder
```
mkdir build && cd build
```

Cmake
```
camke ..
```

Build 
```
make
```

Run
```
./doorlockTest
```
After running the code, you should see the LED light up for 1.5 second and turn off afterwards
