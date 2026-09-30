
# Advanced Encryption Standard (AES)

C++ code implementing the Advanced Encryption Standard (AES) block cipher as specified in the FIPS Publication 197 by the NIST.

## Overview

This code implements the Advanced Encryption Standard (AES) block cipher as described in the original FIPS Publication 197 by the NIST. Hence, only the bare block cipher is implemented. This might then be combined with one of the many block modes of operation to encrypt data of arbitrary length.

Note: This implementation comes with no warranty and should not be used in mission critical scenarios.

## Build

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

### Tests

To run the build against test vectors from the NIST's *Cryptographic Algorithm Validation Program*, configure the build with the `TESTS` option enabled:

```bash
mkdir build && cd build
cmake .. -DTESTS=ON
cmake --build .

ctest --output-on-failure
```

### Examples

To build an example application that computes example vectors as shown in the Appendices of the original FIPS Publication 197, configure the build with the `EXAMPLES` option enabled.

```bash
mkdir build && cd build
cmake .. -DEXAMPLES=ON
cmake --build .

./examples/example_vectors
```

## References

"Announcing the ADVANCED ENCRYPTION STANDARD (AES)". Federal Information Processing Standards Publication 197. United States National Institute of Standards and Technology (NIST). DOI: [10.6028/NIST.FIPS.197](https://doi.org/10.6028/NIST.FIPS.197).
