# Testing AS608 Fingerprint Sensor

# Setup
Connect your Fingerprint Sensor according to the instructions [here](../../../README.md)

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
./as608test
```
After running the code, place your finger to the fingerprint sensor, you should see all test passed.
