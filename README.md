# suzuki_sdl

A minimal C library to build and parse Suzuki Serial Data Line (SDL) protocol messages.

It consists of one header file:

- `suzuki_sdl/suzuki_sdl.h`: SDL message + calculations.
- `suzuki_sdl/vehicles.h`: Car/ECU specific data like OBD Address maps.

## Building

The project is built with `CMake`.

```bash
# Without tests
cmake -B build -DBUILD_TESTING=OFF
# With tests
cmake -B build -DBUILD_TESTING=ON

# Build
cmake --build build

# Install to a dir
cmake --install build --prefix ./out

# Run tests
ctest --test-dir build -V
```

## Use from other projects

The project can be used in other `CMake` projects through `FetchContent`.

## Tests

Unity is used for testing and covers both the main protocol and calculations.

See the Build section above for building with test binaries and running them.
