# libsuzuki

A minimal C library to build and parse Suzuki Serial Data Line (SDL) protocol messages.

It consists of the following header files:

- `libsuzuki/lib.h`: SDL protocol message.
- `libsuzuki/calc.h`: Calculations for SDL values into processed values.
- `libsuzuki/vehicles/*.h`: Car/ECU specific data like OBD Address maps.

## Building

The project is built with `CMake` and can be used in other `CMake` projects
through `FetchContent`.

## Tests

Unity is used for testing and covers both the main protocol and calculations.
