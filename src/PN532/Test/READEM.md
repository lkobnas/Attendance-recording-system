# Testing PN532 NFC/RFID Reader

# Setup
Connect your NFC/RFID Reader according to the instructions [here](../../../README.md)

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
./pn532test
```

After running the code, place your student card to the RFID Reader, you should see all test passed.
