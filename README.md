# SAE J1939 CAN stack library in C

## What is SAE J1039?
https://en.wikipedia.org/wiki/SAE_J1939

## Functionality
This library will handle:
- CAN 2.0b (with future adaptation to CAN-FD)
- Address Claim procedure
- Multi Packet Transport Protocol procedure
- Core Scheduler
- Sending and receiving 29bit extended CAN frames


## Tools
- cmake
- cpputest
- gcovr
- make
- cppcheck

## Build commands for running unit tests
```bash
make clean
make -j utest
```

## Build commands for running unit tests with coverage
```bash
make clean
make -j gcovr
```

## Build commands for library generation
TODO


## TODO
~~Restrict to only signed commits~~
Address Claim procedure