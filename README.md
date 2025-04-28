## Fixed-point number library

a header-only C++ library for fixed-point arithmetic operations. provides precise numberic representation, suitable for environments where floating-point operations are expensive or unavailable. 

## Features

- template-based implementation with configurable integer and fractional bit counts
- support for both signed and unsigned integer representation
- operator overloading for intuitive arithmetic operations
- type conversions to/from standard floating-point types
- compile-time constants for commonly used values
- type-safe implementation using C++ 20 concepts
- mathematical operations (sign, absolute value)
- compile-time test suite 

## How to run:

```
chmod +x build.sh
./build.sh          # configure + build
./build.sh run      # configure + build + run
./build.sh clean    # clean only
./build.sh rebuild  # clean + configure + build
```
